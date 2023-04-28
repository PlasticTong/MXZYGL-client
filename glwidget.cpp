#include "glwidget.h"
#include <QOpenGLFunctions>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "OpenFBX/ofbx.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    vertices = NULL;
    normals = NULL;
    texCoords = NULL;
    tangents = NULL;
    indices = NULL;

    vboVertices = NULL;
    vboNormals = NULL;
    vboTexCoords = NULL;
    vboTangents = NULL;
    vboIndices = NULL;

    shaderProgram = NULL;
    vertexShader = NULL;
    fragmentShader = NULL;
    currentShader = 0;
    bgBlack = true;
    zoom = 0.0;
    fpsCounter = 0;
}

GLWidget::~GLWidget()
{
    destroyVBOs();
    destroyShaders();
}

void GLWidget::renderOffFile(QString filepath)
{
    makeCurrent();
//    useTexture(0);
//    useShader(0);

    readOFFFile(filepath);

    genNormals();
    genTexCoordsCylinder();
    genTangents();

    createVBOs();
    createShaders();

    updateGL();
    doneCurrent();
}

void GLWidget::renderObjFile(QString filepath)
{
    makeCurrent();
//    useTexture(0);
//    useShader(0);

    readOBJFile(filepath);

//    genNormals();
//    genTexCoordsCylinder();
//    genTangents();

    createVBOs();
    createShaders();

    updateGL();
    doneCurrent();
}

void GLWidget::renderFbxFile(QString filepath)
{
    makeCurrent();
//    useTexture(0);
//    useShader(0);

    readFBXFile(filepath);

    if (nullptr==this->normals) genNormals();
//    genTexCoordsCylinder();
//    genTangents();

    createVBOs();
    createShaders();

    updateGL();
    doneCurrent();
}

void GLWidget::initializeGL()
{
    useTexture(0);
    useShader(1);

    connect (& timer , SIGNAL ( timeout () ) , this , SLOT ( animate () ));
    timer.start (100);
}

void GLWidget::resizeGL(int width, int height)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glViewport(0, 0, width , height);
    projectionMatrix.setToIdentity ();
    projectionMatrix.perspective(
        60.0,
        static_cast<qreal>(width) / static_cast<qreal>(height),
        0.1,
        20.0
    );

    trackBall.resizeViewport(width , height);

    updateGL();
}

void GLWidget::paintGL()
{
//    glClearColor(1, 1, 1, 1);
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glClearColor(0.3,0.3,0.3,1.0);
    glFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!vboVertices)
        return;

    modelViewMatrix.setToIdentity ();
    modelViewMatrix.lookAt(camera.eye ,camera.at ,camera.up);
    modelViewMatrix.translate (0, 0, zoom);
    modelViewMatrix.rotate(trackBall.getRotation ());

    shaderProgram ->bind ();

    shaderProgram ->setUniformValue("modelViewMatrix", modelViewMatrix);
    shaderProgram ->setUniformValue("projectionMatrix", projectionMatrix);
    shaderProgram ->setUniformValue("normalMatrix", modelViewMatrix.normalMatrix ());

    QVector4D ambientProduct = light.ambient * material.ambient;
    QVector4D diffuseProduct = light.diffuse * material.diffuse;
    QVector4D specularProduct = light.specular * material.specular;

    shaderProgram ->setUniformValue("lightPosition", light.position);
    shaderProgram ->setUniformValue("ambientProduct", ambientProduct);
    shaderProgram ->setUniformValue("diffuseProduct", diffuseProduct);
    shaderProgram ->setUniformValue("specularProduct", specularProduct);
    shaderProgram ->setUniformValue("shininess", static_cast <GLfloat >( material.shininess));
    shaderProgram ->setUniformValue("texColorMap", 0);
    shaderProgram ->setUniformValue("texNormalMap", 1);

    glFuncs.glActiveTexture ( GL_TEXTURE0 );
    glFuncs.glBindTexture ( GL_TEXTURE_2D , texID [0]) ;
    glFuncs.glActiveTexture ( GL_TEXTURE1 );
    glFuncs.glBindTexture ( GL_TEXTURE_2D , texID [1]) ;

    vboVertices ->bind();
    shaderProgram ->enableAttributeArray("vPosition");
    shaderProgram ->setAttributeBuffer("vPosition", GL_FLOAT ,0, 4, 0);
    vboNormals ->bind ();
    shaderProgram ->enableAttributeArray("vNormal");
    shaderProgram ->setAttributeBuffer("vNormal", GL_FLOAT ,0, 3, 0);
    vboTexCoords ->bind ();
    shaderProgram ->enableAttributeArray("vTexCoord");
    shaderProgram ->setAttributeBuffer("vTexCoord", GL_FLOAT ,0, 2, 0);
    vboTangents ->bind();
    shaderProgram ->enableAttributeArray("vTangent");
    shaderProgram ->setAttributeBuffer("vTangent", GL_FLOAT ,0, 4, 0);
    vboIndices ->bind ();

//    glDrawElements(GL_TRIANGLES , numFaces * 3, GL_UNSIGNED_INT , 0);
    glFuncs.glDrawElements(GL_TRIANGLES , numFaces * 3, GL_UNSIGNED_INT , 0);

    vboIndices ->release ();
    vboTangents ->release ();
    vboTexCoords ->release ();
    vboNormals ->release ();
    vboVertices ->release ();
    shaderProgram ->release ();
}

void GLWidget :: animate ()
{
    updateGL();
}

void GLWidget::toggleBackgroundColor()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    if (bgBlack)
    {
        glFuncs.glClearColor(1, 1, 1, 1);
        bgBlack = false;
    }
    else
    {
        glFuncs.glClearColor(0, 0, 0, 1);
        bgBlack = true;
    }

    updateGL();
}

void GLWidget::useTexture(int textureIndex)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glEnable ( GL_DEPTH_TEST );

    QString textures[] = {
        ":/assets/textures/bricksDiffuse.png",
        ":/assets/textures/bricksNormal.png",
        ":/assets/textures/wood.jpeg",
        ":/assets/textures/rocks.jpg",
        ":/assets/textures/illuminati-eye.jpg"
    };

    QImage texColor = QImage (textures[textureIndex]);
    QImage texNormal = QImage (textures[textureIndex]);
    glFuncs.glActiveTexture ( GL_TEXTURE0 );
    deleteTexture(texID [0]);
    texID [0] = bindTexture ( texColor );
    glFuncs.glActiveTexture ( GL_TEXTURE1 );
    deleteTexture(texID [1]);
    texID [1] = bindTexture ( texNormal );
}

void GLWidget::useShader(int shaderIndex)
{
    currentShader = shaderIndex;
    createShaders();
    updateGL();
}

void GLWidget::readOFFFile(const QString &fileName)
{
    qDebug() <<"GLWidget::readOFFFile"<<fileName;
    std::ifstream stream;
    stream.open(fileName.toLatin1(), std::ifstream::in);

    if (!stream.is_open())
    {
        qWarning("Could not open file");
        return;
    }

    std::string line;

    stream >> line;
    stream >> numVertices >> numFaces >> line;
    qDebug() <<"GLWidget::readOFFFile"<<numVertices;

    delete[] vertices;
    vertices = new QVector4D[numVertices];

    delete[] indices;
    indices = new unsigned int[numFaces * 3];

    if (numVertices > 0)
    {
        float minLim = std::numeric_limits<float>::min();
        float maxLim = std::numeric_limits<float>::max();
        QVector4D max(minLim, minLim, minLim, 1.0);
        QVector4D min(maxLim, maxLim, maxLim, 1.0);

        for (unsigned int i = 0; i < numVertices; i++)
        {
            float x, y, z;
            stream >> x >> y >> z;

            max.setX(qMax(max.x(), x));
            max.setY(qMax(max.y(), y));
            max.setZ(qMax(max.z(), z));

            min.setX(qMin(min.x(), x));
            min.setY(qMin(min.y(), y));
            min.setZ(qMin(min.z(), z));

            vertices[i] = QVector4D(x, y, z, 1.0);
        }

        QVector4D midpoint = (min + max) / 2;
        float invdiag = 1 / (max - min).length();

        for (unsigned int i = 0; i < numVertices; i++)
        {
            vertices[i] = (vertices[i] - midpoint) * invdiag;
            vertices[i].setW(1);
        }
    }

    for (unsigned int i = 0; i < numFaces; i++)
    {
        unsigned int a, b, c;
        stream >> line >> a >> b >> c;
        indices[i * 3 + 0] = a;
        indices[i * 3 + 1] = b;
        indices[i * 3 + 2] = c;
    }

    stream.close();

    emit statusBarMessage (
        QString (" Samples %1 , Faces %2 ")
        .arg(numVertices)
        .arg(numFaces)
    );
}

void GLWidget::readOBJFile(const QString &fileName)
{
    qDebug() <<"GLWidget::readOBJFile"<<fileName;

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(fileName.toStdString(), reader_config)) {
      if (!reader.Error().empty()) {
          std::cerr << "TinyObjReader: " << reader.Error();
      }
      exit(1);
    }


    if (!reader.Warning().empty()) {
      std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    numFaces=0;
    numVertices=attrib.vertices.size();
    for(auto& shape : shapes)
    {
        numFaces += shape.mesh.num_face_vertices.size();
    }

//    auto& shape = shapes[0];

//    numFaces = shape.mesh.num_face_vertices.size();
//    numVertices = attrib.vertices.size();

    delete[] indices;
    indices = new uint32_t[numFaces * 3];
    delete[] vertices;
    vertices = new QVector4D[numVertices];
    delete[] normals;
    normals = new QVector3D[numVertices];
    delete[] texCoords;
    texCoords = new QVector2D[numVertices];

    float minLim = std::numeric_limits<float>::min();
    float maxLim = std::numeric_limits<float>::max();
    QVector4D max(minLim, minLim, minLim, 1.0);
    QVector4D min(maxLim, maxLim, maxLim, 1.0);

    size_t index_offset_shape = 0;
    for (auto &shape : shapes) {
        size_t index_offset = 0;
        int nfaces = shape.mesh.num_face_vertices.size();
        for (size_t f = 0; f < nfaces; f++) {
            size_t fv = size_t(shape.mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                auto v_indi = idx.vertex_index;

                indices[index_offset_shape + index_offset + v] = v_indi;

                float vx = attrib.vertices[3 * size_t(v_indi) + 0];
                float vy = attrib.vertices[3 * size_t(v_indi) + 1];
                float vz = attrib.vertices[3 * size_t(v_indi) + 2];

                vertices[v_indi] = {vx, vy, vz, 1.0};

                max.setX(qMax(max.x(), vx));
                max.setY(qMax(max.y(), vy));
                max.setZ(qMax(max.z(), vz));
                min.setX(qMin(min.x(), vx));
                min.setY(qMin(min.y(), vy));
                min.setZ(qMin(min.z(), vz));

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    float nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    float ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    float nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    normals[v_indi] += {nx, ny, nz};
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    float tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    float ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    texCoords[v_indi] = {tx, ty};
                }
            }
            index_offset += fv;
        }
        index_offset_shape += index_offset;
    }

    for (unsigned int i = 0; i < numVertices; i++)
        normals[i].normalize();

    QVector4D midpoint = (min + max) / 2;
    float invdiag = 1 / (max - min).length();

    for (unsigned int i = 0; i < numVertices; i++)
    {
        vertices[i] = (vertices[i] - midpoint) * invdiag;
        vertices[i].setW(1);
    }

    emit statusBarMessage (
        QString (" Samples %1 , Faces %2 ")
        .arg(numVertices)
        .arg(numFaces)
    );
}

void GLWidget::readFBXFile(const QString &fileName)
{
    qDebug() <<"GLWidget::readFbxFile"<<fileName;

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");

    if (!fp) {
        qDebug() <<"GLWidget::readFbxFile: cannot open file";
        return;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    auto* content = new ofbx::u8[file_size];
//    auto* content = (ofbx::u8*)malloc(file_size*sizeof(ofbx::u8));
    fread(content, 1, file_size, fp);

    // Ignoring certain nodes will only stop them from being processed not tokenised (i.e. they will still be in the tree)
    ofbx::LoadFlags flags =
        ofbx::LoadFlags::TRIANGULATE |
//		ofbx::LoadFlags::IGNORE_MODELS |
        ofbx::LoadFlags::IGNORE_BLEND_SHAPES |
        ofbx::LoadFlags::IGNORE_CAMERAS |
        ofbx::LoadFlags::IGNORE_LIGHTS |
        ofbx::LoadFlags::IGNORE_TEXTURES |
        ofbx::LoadFlags::IGNORE_SKIN |
        ofbx::LoadFlags::IGNORE_BONES |
        ofbx::LoadFlags::IGNORE_PIVOTS |
        ofbx::LoadFlags::IGNORE_MATERIALS |
        ofbx::LoadFlags::IGNORE_POSES |
        ofbx::LoadFlags::IGNORE_VIDEOS |
        ofbx::LoadFlags::IGNORE_LIMBS |
//		ofbx::LoadFlags::IGNORE_MESHES |
        ofbx::LoadFlags::IGNORE_ANIMATIONS;

    ofbx::IScene *fbxscene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u16)flags);

    auto nmesh = fbxscene->getMeshCount();
    bool has_normal_all = true;
    bool has_uv_all = true;

    numVertices=0;
    numFaces=0;

    for (int i=0;i<nmesh;i++) {
        auto geom = fbxscene->getMesh(i)->getGeometry();
        if(geom->getNormals()==nullptr) has_normal_all=false;
        if(geom->getUVs()==nullptr)     has_uv_all=false;

        auto nVert = geom->getVertexCount();
        auto nFace = geom->getIndexCount() / 3;
        this->numVertices   +=  nVert;
        this->numFaces      +=  nFace;
    }

    delete[] this->vertices;
    delete[] this->indices;
    delete[] this->normals;
    delete[] this->texCoords;
    this->vertices  = new QVector4D[numVertices];
    this->indices   = new uint32_t[numFaces * 3];
    this->normals   = has_normal_all ? new QVector3D[numVertices] : nullptr;
    this->texCoords = has_uv_all     ? new QVector2D[numVertices] : nullptr;

    float minLim = std::numeric_limits<float>::min();
    float maxLim = std::numeric_limits<float>::max();
    QVector4D max(minLim, minLim, minLim, 1.0);
    QVector4D min(maxLim, maxLim, maxLim, 1.0);

    // multiple meshes merge into a batch
    int vid_offset = 0;
    int idx_offset = 0;
    for (int i=0;i<nmesh;i++) {
        auto geom = fbxscene->getMesh(i)->getGeometry();

        auto nIndi = geom->getIndexCount();

        auto localTrans = geom->getGlobalTransform();
        QMatrix4x4 localMatV, localMatN;
        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++){
                float mv = static_cast<float>(localTrans.m[i*4+j]);
                localMatV.data()[i*4+j] = mv;
                if (i<3 && j<3)
                    localMatN.data()[i*4+j] = mv;
            }
        }

        auto indi =  geom->getFaceIndices();
        auto vert = geom->getVertices();
        auto norm = geom->getNormals();
        auto uv = geom->getUVs();

        for (int index = 0; index < nIndi; index++) {
            auto vid_current = indi[index] >= 0 ? indi[index] : -indi[index] - 1;
            auto vid_total = vid_current + vid_offset;

            // vertices
            auto v = vert[vid_current];
            float vx = static_cast<float>(v.x);
            float vy = static_cast<float>(v.y);
            float vz = static_cast<float>(v.z);
            this->vertices[vid_total] = localMatV * QVector4D{vx,vy,vz,1.f};
            auto rv = this->vertices[vid_total];
            vx=rv.x();
            vy=rv.y();
            vz=rv.z();

            max.setX(qMax(max.x(), vx));
            max.setY(qMax(max.y(), vy));
            max.setZ(qMax(max.z(), vz));
            min.setX(qMin(min.x(), vx));
            min.setY(qMin(min.y(), vy));
            min.setZ(qMin(min.z(), vz));

            // normals
            if (has_normal_all) {
                if (nullptr != norm){
                    auto vn = norm[vid_current];
                    this->normals[vid_total] += localMatN * QVector3D{
                        static_cast<float>(vn.x),
                        static_cast<float>(vn.y),
                        static_cast<float>(vn.z)
                    };
                }
            }
            // texCoords
            if (has_uv_all) {
                if (nullptr != uv) {
                    auto vt = uv[vid_current];
                    this->texCoords[vid_total] = {
                        static_cast<float>(vt.x),
                        static_cast<float>(vt.y)
                    };
                }
            }
            this->indices[index + idx_offset]=vid_total;
        }
        vid_offset += geom->getVertexCount();
        idx_offset += nIndi;
    }

    if (has_normal_all)
        for (uint32_t i=0; i<numVertices; i++)
            this->normals[i].normalize();

    QVector4D midpoint = (min + max) / 2;
    float invdiag = 1 / (max - min).length();

    for (unsigned int i = 0; i < numVertices; i++)
    {
        vertices[i] = (vertices[i] - midpoint) * invdiag;
        vertices[i].setW(1);
    }

    delete[] content;
//    free(content);
    fclose(fp);

    emit statusBarMessage (
        QString (" Vertices %1 , Faces %2 ")
        .arg(numVertices)
        .arg(numFaces)
    );
}

void GLWidget::genNormals()
{
    delete[] normals;
    normals = new QVector3D[numVertices];

    for (unsigned int i = 0; i < numFaces; i++)
    {
        unsigned int i1 = indices[i * 3 + 0];
        unsigned int i2 = indices[i * 3 + 1];
        unsigned int i3 = indices[i * 3 + 2];

        QVector3D v1 = vertices[i1].toVector3D();
        QVector3D v2 = vertices[i2].toVector3D();
        QVector3D v3 = vertices[i3].toVector3D();

        QVector3D faceNormal = QVector3D::crossProduct(v2 - v1, v3 - v1);
        normals[i1] += faceNormal;
        normals[i2] += faceNormal;
        normals[i3] += faceNormal;
    }

    for (unsigned int i = 0; i < numVertices; i++)
        normals[i].normalize();
}

void GLWidget::genTexCoordsCylinder()
{
    delete[] texCoords;
    texCoords = new QVector2D[numVertices];

    double minLim = std::numeric_limits<float>::min();
    double maxLim = std::numeric_limits<float>::max();
    QVector2D max(minLim, minLim);
    QVector2D min(maxLim, maxLim);

    for (unsigned int i = 0; i < numVertices; i++)
    {
        QVector2D pos = vertices[i].toVector2D();
        max.setX(qMax(max.x(), pos.x()));
        max.setY(qMax(max.y(), pos.y()));
        min.setX(qMin(min.x(), pos.x()));
        min.setY(qMin(min.y(), pos.y()));
    }

    QVector2D size = max - min;

    for (unsigned int i = 0; i < numVertices; i++)
    {
        float x = 2.0 * (vertices[i].x() - min.x()) / size.x() - 1.0;
        texCoords[i] = QVector2D(acos(x) / M_PI, (vertices[i].y() - min.y()) / size.y());
    }
}

void GLWidget::genTangents()
{
    delete[] tangents;
    tangents = new QVector4D[numVertices];
    QVector3D *bitangents = new QVector3D[numVertices];

    for (unsigned int i = 0; i < numFaces; i++)
    {
        unsigned int i1 = indices[i * 3 + 0];
        unsigned int i2 = indices[i * 3 + 1];
        unsigned int i3 = indices[i * 3 + 2];

        QVector3D E = vertices[i1].toVector3D();
        QVector3D F = vertices[i2].toVector3D();
        QVector3D G = vertices[i3].toVector3D();

        QVector2D stE = texCoords[i1];
        QVector2D stF = texCoords[i2];
        QVector2D stG = texCoords[i3];

        QVector3D P = F - E;
        QVector3D Q = G - E;

        QVector2D st1 = stF - stE;
        QVector2D st2 = stG - stE;

        QMatrix2x2 M;
        M(0,0) =  st2.y(); M(0,1) = -st1.y();
        M(1,0) = -st2.x(); M(1,1) =  st1.x();
        M *= (1.0 / (st1.x() * st2.y() - st2.x() * st1.y()));

        QVector4D T = QVector4D(
            M(0,0) * P.x() + M(0,1) * Q.x(),
            M(0,0) * P.y() + M(0,1) * Q.y(),
            M(0,0) * P.z() + M(0,1) * Q.z(),
            0.0
        );

        QVector3D B = QVector3D(
            M(1,0) * P.x() + M(1,1) * Q.x(),
            M(1,0) * P.y() + M(1,1) * Q.y(),
            M(1,0) * P.z() + M(1,1) * Q.z()
        );

        tangents[i1] += T;
        tangents[i2] += T;
        tangents[i3] += T;

        bitangents[i1] += B;
        bitangents[i2] += B;
        bitangents[i3] += B;
    }

    for (unsigned int i = 0; i < numVertices; i++)
    {
        const QVector3D &n = normals[i];
        const QVector4D &t = tangents[i];

        tangents[i] = (t - n * QVector3D::dotProduct(n, t.toVector3D())).normalized();

        QVector3D b = QVector3D::crossProduct(n, t.toVector3D());

        double hand = QVector3D::dotProduct(b, bitangents[i]);

        tangents[i].setW(hand < 0.0 ? -1.0 : 1.0);
    }

    delete[] bitangents;
}

void GLWidget :: createShaders ()
{
    destroyShaders () ;

    QString vertexShaderFile [] = {
        ":/assets/shaders/vgouraud.glsl",
        ":/assets/shaders/vphong.glsl",
        ":/assets/shaders/vtexture.glsl",
        ":/assets/shaders/vnormal.glsl"
    };

    QString fragmentShaderFile [] = {
        ":/assets/shaders/fgouraud.glsl",
        ":/assets/shaders/fphong.glsl",
        ":/assets/shaders/ftexture.glsl",
        ":/assets/shaders/fnormal.glsl"
    };

    vertexShader = new QGLShader ( QGLShader :: Vertex );

    if (!vertexShader->compileSourceFile(vertexShaderFile[currentShader]))
        qWarning() << vertexShader->log();

    fragmentShader = new QGLShader ( QGLShader :: Fragment );

    if (!fragmentShader->compileSourceFile(fragmentShaderFile[currentShader]))
        qWarning() << fragmentShader->log();

    shaderProgram = new QGLShaderProgram ;
    shaderProgram -> addShader ( vertexShader );
    shaderProgram -> addShader ( fragmentShader );

    if (! shaderProgram -> link () )
        qWarning () << shaderProgram -> log () ;
}

void GLWidget :: destroyShaders ()
{
    delete vertexShader ;
    vertexShader = NULL ;

    delete fragmentShader ;
    fragmentShader = NULL ;

    if (shaderProgram)
    {
        shaderProgram -> release () ;
        delete shaderProgram ;
        shaderProgram = NULL ;
    }
}

void GLWidget :: createVBOs ()
{
    destroyVBOs () ;

    vboVertices = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboVertices -> create () ;
    vboVertices -> bind () ;
    vboVertices -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboVertices -> allocate ( vertices , numVertices * sizeof ( QVector4D ));

    delete [] vertices ;
    vertices = NULL ;

    vboNormals = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboNormals -> create () ;
    vboNormals -> bind () ;
    vboNormals -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboNormals -> allocate ( normals , numVertices * sizeof ( QVector3D ));

//    delete [] normals ;
    normals = NULL ;

    vboTexCoords = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboTexCoords -> create () ;
    vboTexCoords -> bind () ;
    vboTexCoords -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboTexCoords -> allocate ( texCoords , numVertices * sizeof ( QVector2D ));

    delete [] texCoords ;
    texCoords = NULL ;

    vboTangents = new QGLBuffer ( QGLBuffer :: VertexBuffer );
    vboTangents -> create () ;
    vboTangents -> bind () ;
    vboTangents -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboTangents -> allocate ( tangents , numVertices * sizeof ( QVector4D ));

    delete [] tangents ;
    tangents = NULL ;

    vboIndices = new QGLBuffer ( QGLBuffer :: IndexBuffer );
    vboIndices -> create () ;
    vboIndices -> bind () ;
    vboIndices -> setUsagePattern ( QGLBuffer :: StaticDraw );
    vboIndices -> allocate ( indices , numFaces * 3 * sizeof ( unsigned int ));

    delete [] indices ;
    indices = NULL ;
}

void GLWidget :: destroyVBOs ()
{
    if ( vboVertices )
    {
        vboVertices -> release () ;
        delete vboVertices ;
        vboVertices = NULL ;
    }

    if ( vboNormals )
    {
        vboNormals -> release () ;
        delete vboNormals ;
        vboNormals = NULL ;
    }

    if ( vboTexCoords )
    {
        vboTexCoords -> release () ;
        delete vboTexCoords ;
        vboTexCoords = NULL ;
    }

    if ( vboTangents )
    {
        vboTangents -> release () ;
        delete vboTangents ;
        vboTangents = NULL ;
    }

    if ( vboIndices )
    {
        vboIndices -> release () ;
        delete vboIndices ;
        vboIndices = NULL ;
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event ->key())
    {
    case Qt:: Key_0: useShader(0); break;
    case Qt:: Key_1: useShader(1); break;
    case Qt:: Key_2: useShader(2); break;
    case Qt:: Key_3: useShader(3); break;
    case Qt:: Key_Escape: qApp ->quit ();
    }
}

void GLWidget :: mouseMoveEvent(QMouseEvent *event)
{
    trackBall.mouseMove(event -> pos());
}

void GLWidget :: mousePressEvent(QMouseEvent *event)
{
    if (event ->button () & Qt:: LeftButton)
        trackBall.mousePress(event -> pos());
}

void GLWidget :: mouseReleaseEvent(QMouseEvent *event)
{
    if (event ->button () == Qt:: LeftButton)
        trackBall.mouseRelease(event -> pos());
}

void GLWidget :: wheelEvent(QWheelEvent *event)
{
    zoom += 0.001 * event->delta();
}

//void GLWidget::takeScreenshot()
//{
//    QImage screenshot = grabFrameBuffer () ;

//    QString fileName = QFileDialog :: getSaveFileName(
//        this,
//        "Save File As",
//        ".",
//        QString("PNG Files (*.png)")
//    );

//    if (fileName.length())
//    {
//        if (!fileName.contains(".png")) fileName += ".png";
//        if (screenshot.save(fileName, "PNG"))
//            QMessageBox::information(
//                this,
//                "Screenshot",
//                "Screenshot taken!",
//                QMessageBox::Ok
//            );
//    }
//}

void GLWidget::showOpenFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "../opengl-3d-visualizer/off",
        QString("OFF Files (*.off)"),
        nullptr,
        QFileDialog::DontUseNativeDialog
    );

    if (!fileName.isEmpty())
    {
        readOFFFile(fileName);

        genNormals();
        genTexCoordsCylinder();
        genTangents();

        createVBOs();
        createShaders();

        updateGL();
    }
}

void GLWidget::useShader0() { useShader(0); }
void GLWidget::useShader1() { useShader(1); }
void GLWidget::useShader2() { useShader(2); }
void GLWidget::useShader3() { useShader(3); }

void GLWidget::useTexture0() { useTexture(0); }
void GLWidget::useTexture1() { useTexture(1); }
void GLWidget::useTexture2() { useTexture(2); }
void GLWidget::useTexture3() { useTexture(3); }
void GLWidget::useTexture4() { useTexture(4); }
