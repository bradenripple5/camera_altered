#include "GLVideoRendererYUV420Filter.h"
#include "GLShaders.h"
#include "Log.h"
#include "RendererGlobals.h"

#include <random>

// Global renderer instance

GLVideoRendererYUV420Filter::GLVideoRendererYUV420Filter() {
    g_renderer = this;
    LOGE("g_renderer assigned inside GLVideoRendererYUV420Filter constructor: %p", g_renderer);

    m_fragmentShader.push_back(kFragmentShader0);
    m_fragmentShader.push_back(kFragmentShader);
    m_fragmentShader.push_back(kFragmentShader1);
    m_fragmentShader.push_back(kFragmentShader2);
    m_fragmentShader.push_back(kFragmentShader3);
    m_fragmentShader.push_back(kFragmentShader4);
    m_fragmentShader.push_back(kFragmentShader5);
    m_fragmentShader.push_back(kFragmentShader6);
    m_fragmentShader.push_back(kFragmentShader7);
    m_fragmentShader.push_back(kFragmentShader8);
    m_fragmentShader.push_back(kFragmentShader9);
    m_fragmentShader.push_back(kFragmentShader10);
    m_fragmentShader.push_back(kFragmentShader11);
    m_fragmentShader.push_back(kFragmentShader12);
}

GLVideoRendererYUV420Filter::~GLVideoRendererYUV420Filter() = default;

void GLVideoRendererYUV420Filter::setParameters(uint32_t params) {
    GLVideoRendererYUV420::setParameters(params);
    m_filter = params & 0x0000000F;
}

uint32_t GLVideoRendererYUV420Filter::getParameters() {
    m_params |= (m_fragmentShader.size() << 4) & 0x000000F0;
    return m_params;
}

void GLVideoRendererYUV420Filter::setEffectIntensity(float value) {
    this->m_intensity = value;

//    m_intensity = value;
    LOGE("setEffectIntensity called on %p with value = %f", this, value);
}

void GLVideoRendererYUV420Filter::render() {
    if (m_filter != m_prevFilter) {
        m_prevFilter = m_filter;

        if (m_filter >= 0 && m_filter < m_fragmentShader.size()) {
            isProgramChanged = true;
            delete_program(m_program);
            createProgram(kVertexShader, m_fragmentShader.at(m_filter));
        }
    }

    glUseProgram(m_program);
    GLint intensityHandle = glGetUniformLocation(m_program, "u_threshold");
    LOGE("render called on %p | intensityHandle = %d | m_intensity = %f", this, intensityHandle, m_intensity);
    glUniform1f(intensityHandle, m_intensity);

    GLVideoRendererYUV420::render();
}
