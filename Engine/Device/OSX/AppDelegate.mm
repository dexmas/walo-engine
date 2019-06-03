
#import "Device/OSX/AppDelegate.h"
#import <QuartzCore/CVDisplayLink.h>

#include "Game.hpp"
#include "Core/Log.hpp"

CVDisplayLinkRef m_DisplayLink = nil;

@implementation AppDelegate

@synthesize Window;
@synthesize View;

BOOL m_Active;
NSTimer* m_RenderTimer;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    m_Active = FALSE;
    
    if(!CGame::Instance())
    {
        LOG("WaloEngine Critical: Game instance not created.\n");
        return;
    }
    
    NSRect frame = NSMakeRect(0, 0, 960, 640);
    
    u32 styleMask = NSWindowStyleMaskTitled|NSWindowStyleMaskClosable;
    
    Window = [[NSWindow alloc] initWithContentRect:frame styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
    View = [[CGameView alloc] initWithFrame: frame];
	
	CGame::Instance()->GetDevice()->SetScreenSize(CVector2(frame.size.width, frame.size.height));
    
    [Window setTitle:@"WaloPlayer"];
	[Window setContentView:View];
	[Window setInitialFirstResponder:View];
	[Window makeFirstResponder:View];
    [Window setDelegate:self];
    [Window center];
    [Window makeKeyAndOrderFront:nil];
	
	[View.Context makeCurrentContext];
    
    if(!CGame::Instance()->Init())
    {
        LOG("WaloPlayer: Game initialization failed.\n");
        return;
    }
    
    m_RenderTimer = [NSTimer timerWithTimeInterval:1.0f/1000.0f target:self selector:@selector(Update:) userInfo:nil repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:m_RenderTimer forMode:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:m_RenderTimer forMode:NSEventTrackingRunLoopMode];
}

- (void)windowWillClose:(NSNotification *)aNotification
{
    [NSApp terminate:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (void)applicationWillResignActive:(NSNotification *)aNotification
{
    if(CGame::Instance())
    {
        m_Active = FALSE;
        CGame::Instance()->Activate(false);
    }
}

- (void)applicationDidBecomeActive:(NSNotification *)aNotification
{
    if(CGame::Instance())
    {
        CGame::Instance()->Activate(true);
        m_Active = TRUE;
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    if(CGame::Instance())
    {
        m_Active = FALSE;
        
        CGame::Instance()->Free();
        
        delete CGame::Instance();
    }
}

- (void)Update:(id)sender
{
    if(m_Active)
    {
        [View.Context makeCurrentContext];
        
        if(CGame::Instance())
        {
            CGame::Instance()->Run();
        }
        
        [View.Context flushBuffer];
    }
}

@end
