#ifndef _GL_VIDEO_RENDERER_YUV_FILTER_H_
#define _GL_VIDEO_RENDERER_YUV_FILTER_H_

#include "GLVideoRendererYUV420.h"
#include <vector>

class GLVideoRendererYUV420Filter : public GLVideoRendererYUV420 {
public:
    GLVideoRendererYUV420Filter();
    ~GLVideoRendererYUV420Filter() override;

    void setParameters(uint32_t params) override;
    uint32_t getParameters() override;
    void setThreshold(float value);

    void render() override;


private:
    std::vector<const char *> m_fragmentShader;
    int m_filter = 0;
    int m_prevFilter = -1;
};

#endif //_GL_VIDEO_RENDERER_YUV_FILTER_H_
