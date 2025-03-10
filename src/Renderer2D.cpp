#include "Renderer2D.h"
#include "Util.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct QuadVertex {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec2 textureCoord;
    int textureSlot;
};

static u32 LoadShader(const std::string& vertexFilename, const std::string& fragmentFilename) {
    std::string vertexSource   = Util::ReadEntireFile(vertexFilename);
    std::string fragmentSource = Util::ReadEntireFile(fragmentFilename);

    const char* vertexSourceRaw   = vertexSource.c_str();
    const char* fragmentSourceRaw = fragmentSource.c_str();

    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSourceRaw, NULL);
    glCompileShader(vertexShader);

    {
        int result = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int messageLen    = 0;
            char message[256] = {};

            glGetShaderInfoLog(vertexShader, 256, &messageLen, message);
            std::cout << "WARNING: Failed to compile vertex shader\n" << message << std::endl;

            return 0;
        }
    }

    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSourceRaw, NULL);
    glCompileShader(fragmentShader);

    {
        int result = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int messageLen    = 0;
            char message[256] = {};

            glGetShaderInfoLog(fragmentShader, 256, &messageLen, message);
            std::cout << "WARNING: Failed to compile fragment shader\n" << message << std::endl;
            
            return 0;
        }
    }

    u32 program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

static Texture2D CreateTexture(u32 width, u32 height, const void* pixels) {
    Texture2D texture = {
        .width  = width,
        .height = height,
    };

    glGenTextures(1, &texture.renderID);
    glBindTexture(GL_TEXTURE_2D, texture.renderID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    return texture;
}

static constexpr int VERTICES_PER_QUAD = 6;
static constexpr int MAX_RENDER_COUNT  = 1000;
static constexpr int MAX_TEXTURE_SLOTS = 8;

static u32 vao;
static u32 vbo;
static u32 shader;

static u32 quadCount;
static std::vector<QuadVertex> quadVertexBuffer;

static u32 textureCount;
static Texture2D textureSlots[MAX_TEXTURE_SLOTS];

static glm::mat4 projection;
static Texture2D whiteTexture;

void Renderer2D::Init() {
    std::string version        = (const char*)glGetString(GL_VERSION);
    std::string renderer       = (const char*)glGetString(GL_RENDERER);
    std::string vendor         = (const char*)glGetString(GL_VENDOR);
    std::string shaderVersion  = (const char*)glGetString( GL_SHADING_LANGUAGE_VERSION);

    std::cout << "VERSION:      " << version << std::endl;
    std::cout << "RENDERER:     " << renderer << std::endl;
    std::cout << "VENDOR:       " << vendor << std::endl;
    std::cout << "GLSL VERSION: " << shaderVersion << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader = LoadShader("quad_vertex.glsl", "quad_fragment.glsl");
    glUseProgram(shader);

    vao = 0;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vbo = 0;
    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * VERTICES_PER_QUAD * MAX_RENDER_COUNT, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, textureCoord));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_INT, false, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, textureSlot));

    projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 1.0f);

    {
        int loc = glGetUniformLocation(shader, "u_projection");
        glUniformMatrix4fv(loc, 1, false, glm::value_ptr(projection));
    }

    u32 whitePixels[] = { 0xffffffff };
    whiteTexture      = CreateTexture(1, 1, whitePixels);
}

void Renderer2D::Shutdown() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

Texture2D Renderer2D::LoadTexture(const std::string& filename) {
    int width, height, comp;
    u8* data = stbi_load(filename.c_str(), &width, &height, &comp, 4);

    if (!data) {
        std::cout << "WARNING: Failed to load texture " << filename << std::endl;
        return {};
    }

    Texture2D texture = CreateTexture(width, height, data);
    stbi_image_free(data);

    return texture;
}

void Renderer2D::Clear(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::Flush() {
    for (int i = 0; i < textureCount; i++) {
        glBindTexture(GL_TEXTURE_2D, textureSlots[i].renderID);
        glBindTextureUnit(i, textureSlots[i].renderID);
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(QuadVertex) * quadVertexBuffer.size(), quadVertexBuffer.data());
    glDrawArrays(GL_TRIANGLES, 0, quadVertexBuffer.size());

    quadCount    = 0;
    textureCount = 0;

    quadVertexBuffer.clear();
}

void Renderer2D::DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color) {
    DrawTexture(whiteTexture, position, size, color);
}

void Renderer2D::DrawTexture(Texture2D texture, glm::vec2 position, glm::vec2 size, glm::vec4 color) {
    if (quadCount == MAX_RENDER_COUNT || textureCount == MAX_TEXTURE_SLOTS) {
        Flush();
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    int textureSlot = -1;

    for (int i = 0; i < textureCount; i++) {
        if (textureSlots[i].renderID == texture.renderID) {
            textureSlot = i;
            break;
        }
    }

    if (textureSlot == -1) {
        textureSlot = textureCount;
        textureSlots[textureCount] = texture;
        textureCount += 1;
    }

    constexpr glm::vec4 vertices[] = {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },

        {  0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f },
    };

    constexpr glm::vec2 texCoords[] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },

        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
    };

    for (int i = 0; i < VERTICES_PER_QUAD; i++) {
        QuadVertex vertex = {
            .position     = transform * vertices[i],
            .color        = color,
            .textureCoord = texCoords[i],
            .textureSlot  = textureSlot, 
        };

        quadVertexBuffer.push_back(vertex);
    }

    quadCount += 1;
}