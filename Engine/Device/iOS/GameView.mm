
#import "Device/iOS/GameView.h"

#include <Device/iOS/DeviceIOS.hpp>
#include <Render/OpenGLES/RenderGLES.hpp>

#include <Device/Event.hpp>

@interface CGameView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation CGameView

int nextTouchId = 0;

@synthesize Context;

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (void)dealloc
{
    [self deleteFramebuffer];
    [Context release];
    [super dealloc];
}

- (id)initWithFrame:(CGRect)_frame;
{
    UIScreen* screen = [UIScreen mainScreen];
    
    self = [super initWithFrame: _frame];
    
    if(!self)
        return nil;
    
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    f32 scale = 1.0f;
    
    if([screen respondsToSelector:@selector(scale)])
    {
        scale = [screen scale];
    }
    
    self.contentScaleFactor = scale;
    
    self.multipleTouchEnabled = TRUE;
    self.exclusiveTouch = YES;

    eaglLayer.opaque = TRUE;
    
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                    nil];
    
    Context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if(!Context || ![EAGLContext setCurrentContext: Context])
    {
        [self release];
        return nil;
    }
    
    defaultFramebuffer = 0;
    colorRenderbuffer = 0;
    depthRenderbuffer = 0;
    
    [self createFramebuffer];

    return self;
}

- (void)createFramebuffer
{
    glGenFramebuffers(1, &defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    [Context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
    
    CGame::Instance()->GetDevice()->SetScreenSize(CVector2(framebufferWidth, framebufferHeight));
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG("! Failed to make complete framebuffer object: %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

- (void)deleteFramebuffer
{
    if(Context)
    {
        [EAGLContext setCurrentContext: Context];
        
        if(defaultFramebuffer)
        {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if(colorRenderbuffer)
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
        
        if(depthRenderbuffer)
        {
            glDeleteRenderbuffers(1, &depthRenderbuffer);
            depthRenderbuffer = 0;
        }
    }
}

- (void)preRender
{
    if(Context)
    {
        [EAGLContext setCurrentContext: Context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    }
}

- (void)postRender
{
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [Context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {
        if([Touches objectForKey:touch] == nil)
        {
            [Touches setObject:[NSNumber numberWithInteger:nextTouchId] forKey:[NSValue valueWithPointer:touch]];
            nextTouchId++;
        }
    }
    [self handleTouches:touches ofType:CTouchEvent::ETE_BEGIN withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self handleTouches:touches ofType:CTouchEvent::ETE_MOVE withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self handleTouches:touches ofType:CTouchEvent::ETE_END withEvent:event];
    
    for (UITouch* touch in touches)
    {
        if ([Touches objectForKey:[NSValue valueWithPointer:touch]] != nil)
        {
            [Touches removeObjectForKey:[NSValue valueWithPointer:touch]];
        }
    }
    
    if ([Touches count] == 0)
        nextTouchId = 0;
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self handleTouches:touches ofType:CTouchEvent::ETE_CANCEL withEvent:event];
    
    for(UITouch* touch in touches)
    {
        if ([Touches objectForKey:[NSValue valueWithPointer:touch]] != nil)
        {
            [Touches removeObjectForKey:[NSValue valueWithPointer:touch]];
        }
    }
    
    if([Touches count] == 0)
        nextTouchId = 0;
}

- (void)handleTouches:(NSSet *)touches ofType:(CTouchEvent::ETouchEventType)etype withEvent:(UIEvent *)event
{
    int touchCount = [[event allTouches] count];
    if(touchCount > 5)
        touchCount = 5;
    
    CTouchEvent Event;
    
    Event.TouchCount = touchCount;
    
    if(touchCount == 1)
    {
        UITouch *touch = [touches anyObject];
        CGPoint point = [touch locationInView:self];
        NSNumber *touchId = [Touches objectForKey:[NSValue valueWithPointer:touch]];
        
        CTouchEvent::ETouchEventType type = CTouchEvent::ETE_COUNT;
        
        switch(touch.phase)
        {
            case UITouchPhaseBegan:
                type = CTouchEvent::ETE_BEGIN;
                break;
            case UITouchPhaseEnded:
                type = CTouchEvent::ETE_END;
                break;
            case UITouchPhaseCancelled:
                type = CTouchEvent::ETE_CANCEL;
                break;
            case UITouchPhaseMoved:
                type = CTouchEvent::ETE_MOVE;
                break;
            default:
                return;
        }
        
        Event.Touches[0].TouchType = type;
        Event.Touches[0].X = point.x * [self contentScaleFactor];
        Event.Touches[0].Y = point.y * [self contentScaleFactor];
        Event.Touches[0].Time = f32([touch timestamp]);
        Event.Touches[0].ID = [touchId intValue];
    }
    else
        for(u32 i=0;i<touchCount;i++)
        {
            UITouch *touch = [[[event allTouches] allObjects] objectAtIndex:i];
            CGPoint point = [touch locationInView:self];
            NSNumber *touchId = [Touches objectForKey:[NSValue valueWithPointer:touch]];
            
            CTouchEvent::ETouchEventType type = CTouchEvent::ETE_COUNT;
            
            switch(touch.phase)
            {
                case UITouchPhaseBegan:
                    type = CTouchEvent::ETE_BEGIN;
                    break;
                case UITouchPhaseEnded:
                    type = CTouchEvent::ETE_END;
                    break;
                case UITouchPhaseCancelled:
                    type = CTouchEvent::ETE_CANCEL;
                    break;
                case UITouchPhaseMoved:
                    type = CTouchEvent::ETE_MOVE;
                    break;
                default:
                    return;
            }
            
            Event.Touches[i].TouchType = type;
            Event.Touches[i].X = point.x * [self contentScaleFactor];
            Event.Touches[i].Y = point.y * [self contentScaleFactor];
            Event.Touches[i].Time = f32([touch timestamp]);
            Event.Touches[i].ID = [touchId intValue];
        }
    
        CGame::Instance()->GetSystem<CInputSystem>()->(&Event);
}

@end