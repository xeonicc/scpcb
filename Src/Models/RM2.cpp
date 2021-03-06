#include "RM2.h"
#include <fstream>
#include <filesystem>

#include <Math/Matrix.h>

const PGE::FilePath RM2::opaqueShaderPath = PGE::FilePath::fromStr("SCPCB/GFX/Shaders/RoomOpaque/");
const PGE::FilePath RM2::opaqueNormalMapShaderPath = PGE::FilePath::fromStr("SCPCB/GFX/Shaders/RoomOpaqueNormalMap/");
const PGE::FilePath RM2::alphaShaderPath = PGE::FilePath::fromStr("SCPCB/GFX/Shaders/RoomAlpha/");

const PGE::String RM2::texturePath = "SCPCB/GFX/Map/Textures/";

enum class FileSections {
    Textures = 1,
    VisibleGeometry = 2,
    InvisibleGeometry = 3,
    Waypoint = 4,
    PointLight = 5,
    Spotlight = 6,
    Prop = 7
};

enum class TextureLoadFlag {
    Opaque = 1,
    Alpha = 2
};

static PGE::String readByteString(std::ifstream& inFile) {
    unsigned char len = 0;
    inFile.read((char*)(void*)&len, 1);

    PGE::String retVal = "";
    for (int i = 0; i < (int)len; i++) {
        char chr = '\0';
        inFile.read(&chr, 1);
        retVal = PGE::String(retVal,PGE::String(chr));
    }

    return retVal;
}

RM2::RM2(GraphicsResources* gfxRes, const PGE::String& filename) {
    graphicsResources = gfxRes;

    std::ifstream inFile;
    inFile.open(PGE::FilePath::fromStr(filename).cstr(), std::ios_base::in | std::ios_base::binary);

    union IntBytes {
        int i;
        char c[4];
    };

    union UShortBytes {
        unsigned short i;
        char c[2];
    };

    union FloatBytes {
        float f;
        char c[4];
    };

    union UCharByte {
        unsigned char u;
        char c;
    };

    //header
    PGE::String header = ".RM2";
    char inHeader[4];

    inFile.read(inHeader, 4);

    if (memcmp(inHeader, header.cstr(), 4) != 0) {
        error = RM2Error::InvalidHeader;
    }

    //textures
    char chunkHeader = 0;
    inFile.read(&chunkHeader, 1);

    if ((FileSections)chunkHeader != FileSections::Textures) {
        error = RM2Error::UnexpectedChunk;
    }

    UCharByte textureCount;
    inFile.read(&textureCount.c, 1);

    const PGE::String extension = ".rm2";

    opaqueShader = gfxRes->getShader(opaqueShaderPath, true);
    opaqueModelMatrixConstant = opaqueShader->getVertexShaderConstant("modelMatrix");

    opaqueNormalMapShader = gfxRes->getShader(opaqueNormalMapShaderPath, true);
    opaqueNormalMapModelMatrixConstant = opaqueNormalMapShader->getVertexShaderConstant("modelMatrix");

    //alphaShader = gfxRes->getShader(alphaShaderPath, true);

    PGE::Texture* lightmapTextures[3];
    for (int i = 0; i < 3; i++) {
        const PGE::String lightmapSuffix = "_lm" + PGE::String(i) + ".png";
        PGE::String lightmapName = filename.substr(0, filename.size() - extension.size()) + lightmapSuffix;
        lightmapTextures[i] = gfxRes->getTexture(PGE::FilePath::fromStr(lightmapName));
    }

    std::vector<PGE::Texture*> materialTextures;

    for (int i = 0; i < (int)textureCount.u; i++) {
        PGE::String textureName = readByteString(inFile);

        PGE::FilePath textureFilePath = PGE::FilePath::fromStr(texturePath + textureName + ".jpg");
        if (!textureFilePath.exists()) {
            textureFilePath  = PGE::FilePath::fromStr(texturePath + textureName + ".png");
        }

        PGE::Texture* texture = gfxRes->getTexture(textureFilePath);

        char flag = 0;
        inFile.read(&flag, 1);

        bool isOpaque = (TextureLoadFlag)flag == TextureLoadFlag::Opaque;

        PGE::Shader* shader = alphaShader;
        if (isOpaque) {
            PGE::FilePath normalMapFilePath = PGE::FilePath::fromStr(texturePath + textureName + "_n.jpg");
            if (!normalMapFilePath.exists()) {
                normalMapFilePath = PGE::FilePath::fromStr(texturePath + textureName + "_n.png");
            }

            if (normalMapFilePath.exists()) {
                materialTextures.resize(5);
                materialTextures[4] = gfxRes->getTexture(normalMapFilePath);
                shader = opaqueNormalMapShader;
            } else {
                materialTextures.resize(4);
                shader = opaqueShader;
            }

            materialTextures[0] = lightmapTextures[0];
            materialTextures[1] = lightmapTextures[1];
            materialTextures[2] = lightmapTextures[2];
            materialTextures[3] = texture;
        } else {
            materialTextures.resize(1);
            materialTextures[0] = texture;
        }

        PGE::Material* material = new PGE::Material(shader, materialTextures, isOpaque);

        materials.push_back(material);
    }

    //everything else
    chunkHeader = 0;
    inFile.read(&chunkHeader, 1);

    bool reachedEndOfFile = inFile.eof();
    while (!reachedEndOfFile) {
        bool unrecognizedChunkHeader = false;
        switch ((FileSections)chunkHeader) {
            case FileSections::VisibleGeometry: {
                UCharByte textureIndex;
                inFile.read(&textureIndex.c, 1);

                UShortBytes vertexCount;
                inFile.read(vertexCount.c, 2);

                std::vector<PGE::Vector3f> vertexPositions;
                std::vector<int> indices;

                std::vector<PGE::Vertex> vertices;
                PGE::Vertex tempVertex;

                for (int i = 0; i < vertexCount.i; i++) {
                    FloatBytes inX;
                    FloatBytes inY;
                    FloatBytes inZ;

                    inFile.read(inX.c, 4);
                    inFile.read(inY.c, 4);
                    inFile.read(inZ.c, 4);

                    PGE::Vector4f position = PGE::Vector4f(inX.f, inY.f, inZ.f, 1.f);

                    FloatBytes inDiffU;
                    FloatBytes inDiffV;
                    FloatBytes inLmU;
                    FloatBytes inLmV;

                    inFile.read(inDiffU.c, 4);
                    inFile.read(inDiffV.c, 4);
                    inFile.read(inLmU.c, 4);
                    inFile.read(inLmV.c, 4);

                    PGE::Vector2f diffUv = PGE::Vector2f(inDiffU.f, inDiffV.f);
                    PGE::Vector2f lmUv = PGE::Vector2f(inLmU.f, inLmV.f);

                    tempVertex.setVector4f("position", position);
                    tempVertex.setVector3f("normal", PGE::Vector3f::one);
                    tempVertex.setVector2f("diffUv", diffUv);
                    tempVertex.setVector2f("lmUv", lmUv);
                    tempVertex.setColor("color", PGE::Color(255,255,255,255));
                    vertices.push_back(tempVertex);

                    vertexPositions.push_back(PGE::Vector3f(position.x, position.y, position.z));
                }

                UShortBytes triangleCount;
                inFile.read(triangleCount.c, 2);

                std::vector<PGE::Primitive> primitives;
                for (int i = 0; i < triangleCount.i; i++) {
                    UShortBytes index0;
                    UShortBytes index1;
                    UShortBytes index2;

                    inFile.read(index0.c, 2);
                    inFile.read(index2.c, 2);
                    inFile.read(index1.c, 2);

                    primitives.push_back(PGE::Primitive(index0.i, index1.i, index2.i));

                    indices.push_back(index0.i);
                    indices.push_back(index1.i);
                    indices.push_back(index2.i);
                }

                PGE::Mesh* mesh = PGE::Mesh::create(gfxRes->getGraphics(), PGE::Primitive::TYPE::TRIANGLE);

                mesh->setMaterial(materials[textureIndex.u]);
                mesh->setGeometry((int)vertices.size(), vertices, (int)primitives.size(), primitives);

                if (materials[textureIndex.u]->isOpaque())
                {
                    opaqueMeshes.push_back(mesh);
                }
                else
                {
                    alphaMeshes.push_back(mesh);
                }

                collisionMeshes.push_back(new CollisionMesh(vertexPositions, indices));
            } break;
            case FileSections::InvisibleGeometry: {
                UShortBytes vertexCount;
                inFile.read(vertexCount.c, 2);

                std::vector<PGE::Vector3f> vertexPositions;
                std::vector<int> indices;

                for (int i = 0; i < vertexCount.i; i++) {
                    FloatBytes inX;
                    FloatBytes inY;
                    FloatBytes inZ;

                    inFile.read(inX.c, 4);
                    inFile.read(inY.c, 4);
                    inFile.read(inZ.c, 4);

                    PGE::Vector3f position = PGE::Vector3f(inX.f, inY.f, inZ.f);

                    vertexPositions.push_back(position);
                }

                UShortBytes triangleCount;
                inFile.read(triangleCount.c, 2);

                for (int i = 0; i < triangleCount.i; i++) {
                    UShortBytes index0;
                    UShortBytes index1;
                    UShortBytes index2;

                    inFile.read(index0.c, 2);
                    inFile.read(index1.c, 2);
                    inFile.read(index2.c, 2);

                    indices.push_back(index0.i);
                    indices.push_back(index1.i);
                    indices.push_back(index2.i);
                }

                collisionMeshes.push_back(new CollisionMesh(vertexPositions, indices));
            } break;
            default: {
                unrecognizedChunkHeader = true;
            } break;
        }

        if (unrecognizedChunkHeader) {
            break; //TODO: throw error here once everything's implemented
        }

        inFile.read(&chunkHeader, 1);

        reachedEndOfFile = inFile.eof();
    }

    inFile.close();
}

void RM2::render(const PGE::Matrix4x4f& modelMatrix) {
    opaqueModelMatrixConstant->setValue(modelMatrix);
    opaqueNormalMapModelMatrixConstant->setValue(modelMatrix);
    for (int i = 0; i < opaqueMeshes.size(); i++) {
        opaqueMeshes[i]->render();
    }

    /*alphaModelMatrixConstant->setValue(modelMatrix);
    //TODO: sort based on distance to camera
    for (int i = 0; i < alphaMeshes.size(); i++) {
        alphaMeshes[i]->render();
    }*/
}

const std::vector<CollisionMesh*>& RM2::getCollisionMeshes() const {
    return collisionMeshes;
}
