#pragma once

#import <UIKit/UIKit.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <Game.hpp>

class CDeviceIOS;

@interface CGameView: UIView
{
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    NSMutableDictionary* Touches;
}

@property (nonatomic, retain) EAGLContext* Context;

- (id)initWithFrame:(CGRect)_frame;

- (void)preRender;
- (void)postRender;

@end