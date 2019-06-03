
#include <Render/OpenGL/RenderGL.hpp>

#include <Device/OSX/DeviceOSX.hpp>
#import <Device/OSX/GameView.h>

#include "Scene/Input/InputSystem.hpp"

#include "Game.hpp"
#include "Core/Log.hpp"

@implementation CGameView

@synthesize Context;

NSTrackingArea* trackingArea;
CMouseEvent mouseEevent(CMouseEvent::EME_MOUSE_MOVED);
CTouchEvent touchEevent;
CKeyboardEvent keyboardEvent;

- (id)initWithFrame:(NSRect)_frame
{
    NSOpenGLPixelFormatAttribute pixelAttrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        0,
    };
    
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelAttrs];
    
    if(!pixelFormat)
    {
        return nil;
    }
	
	self = [super initWithFrame:_frame pixelFormat:pixelFormat];
    
    Context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:NULL];
    
    if(!Context)
    {
        return nil;
    }
    
    [Context makeCurrentContext];
	[self setOpenGLContext:Context];
	
	[Context setView:self];
    
    NSTrackingAreaOptions trackingOptions = NSTrackingMouseMoved|NSTrackingActiveInKeyWindow|NSTrackingInVisibleRect;
    
    trackingArea = [[NSTrackingArea alloc] initWithRect:_frame options:trackingOptions owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
    
    touchEevent.TouchCount = 1;
    touchEevent.Touches[0].ID = 0;

    return self;
}

- (void) mouseDragged:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
    mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
    mouseEevent.X = eventLocation.x;
    mouseEevent.Y = self.frame.size.height - eventLocation.y;
        
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
    
    if((mouseEevent.Buttons & CMouseEvent::EMBM_LEFT) == CMouseEvent::EMBM_LEFT)
    {
        touchEevent.Touches[0].TouchType = CTouchEvent::ETE_MOVE;
        touchEevent.Touches[0].X = eventLocation.x;
        touchEevent.Touches[0].Y = self.frame.size.height - eventLocation.y;
        touchEevent.Touches[0].Time = u32(CGame::Instance()->GetTimer()->GetCurentTime());
        
        CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&touchEevent);
    }
}

- (void) mouseUp:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    mouseEevent.Buttons &= ~CMouseEvent::EMBM_LEFT;
    mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
    mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
    mouseEevent.X = eventLocation.x;
    mouseEevent.Y = self.frame.size.height - eventLocation.y;
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
    
    touchEevent.Touches[0].TouchType = CTouchEvent::ETE_END;
    touchEevent.Touches[0].X = eventLocation.x;
    touchEevent.Touches[0].Y = self.frame.size.height - eventLocation.y;
    touchEevent.Touches[0].Time = u32(CGame::Instance()->GetTimer()->GetCurentTime());
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&touchEevent);
}

- (void) mouseDown:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    mouseEevent.Buttons |= CMouseEvent::EMBM_LEFT;
    mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
    mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
    mouseEevent.X = eventLocation.x;
    mouseEevent.Y = self.frame.size.height - eventLocation.y;
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
    
    touchEevent.Touches[0].TouchType = CTouchEvent::ETE_BEGIN;
    touchEevent.Touches[0].X = eventLocation.x;
    touchEevent.Touches[0].Y = self.frame.size.height - eventLocation.y;
    touchEevent.Touches[0].Time = u32(CGame::Instance()->GetTimer()->GetCurentTime());
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&touchEevent);
}

- (void) rightMouseDragged:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    if(mouseEevent.Buttons)
    {
        mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
        mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
        mouseEevent.X = eventLocation.x;
        mouseEevent.Y = self.frame.size.height - eventLocation.y;
        
        CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
    }
}

- (void) rightMouseUp:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    mouseEevent.Buttons &= ~CMouseEvent::EMBM_RIGHT;
    mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
    mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
    mouseEevent.X = eventLocation.x;
    mouseEevent.Y = self.frame.size.height - eventLocation.y;
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
}

- (void) rightMouseDown:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    mouseEevent.Buttons |= CMouseEvent::EMBM_RIGHT;
    mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
    mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
    mouseEevent.X = eventLocation.x;
    mouseEevent.Y = self.frame.size.height - eventLocation.y;
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
}

- (void) scrollWheel:(NSEvent *)event
{
    NSPoint eventLocation = [self convertPoint:event.locationInWindow fromView:nil];
    
    mouseEevent.Wheel = event.scrollingDeltaX;
    mouseEevent.Control = [event modifierFlags] & NSCommandKeyMask;
    mouseEevent.Shift = [event modifierFlags] & NSShiftKeyMask;
    mouseEevent.X = eventLocation.x;
    mouseEevent.Y = self.frame.size.height - eventLocation.y;
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mouseEevent);
}

- (void) keyUp:(NSEvent *)event
{
    keyboardEvent.PressedDown = false;
    keyboardEvent.Control = [event modifierFlags] & NSCommandKeyMask;
    keyboardEvent.Shift = [event modifierFlags] & NSShiftKeyMask;
    keyboardEvent.Char = [event.characters UTF8String][0];
    keyboardEvent.Key = [self mapKeyCode: event.keyCode];
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&keyboardEvent);
}

- (void) keyDown:(NSEvent *)event
{
    keyboardEvent.PressedDown = true;
    keyboardEvent.Control = [event modifierFlags] & NSCommandKeyMask;
    keyboardEvent.Shift = [event modifierFlags] & NSShiftKeyMask;
    keyboardEvent.Char = [event.characters UTF8String][0];
    keyboardEvent.Key = [self mapKeyCode: event.keyCode];
    
    CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&keyboardEvent);
}

- (CKeyboardEvent::EKeyCode) mapKeyCode:(unsigned short)_code
{
    return (CKeyboardEvent::EKeyCode)_code;
}

@end
