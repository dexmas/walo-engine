#pragma once

#import <Cocoa/Cocoa.h>

#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>

@interface CGameView: NSOpenGLView
{
    
}

@property (nonatomic, retain) NSOpenGLContext* Context;

- (id)initWithFrame:(NSRect)_frame;

@end
