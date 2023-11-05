#pragma once

#import <MetalKit/MetalKit.h>

namespace o2
{
    struct RenderDevice
    {
    public:
        // Initializes renderer
        static void Initialize();
        
    public:
        static MTKView*                    view;
        static id<MTLDevice>               device;
        static id<MTLLibrary>              defaultLibrary;
        static id<MTLCommandQueue>         commandQueue;
        static id<MTLRenderPipelineState>  pipelineState;
        static id<MTLRenderCommandEncoder> renderEncoder;
        static id<MTLCommandBuffer>        commandBuffer;
        
        static id<MTLBuffer> vertexBuffers[2];
        static id<MTLBuffer> indexBuffers[2];
        static id<MTLBuffer> uniformBuffers[2];
        
        static id<MTLBuffer> vertexBuffer;
        static id<MTLBuffer> indexBuffer;
        static id<MTLBuffer> uniformBuffer;
    };
    
    struct MTLTextureImpl
    {
        id<MTLTexture> texture;
    };
}
