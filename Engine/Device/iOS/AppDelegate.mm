
#import "Device/iOS/AppDelegate.h"

#include "Game.hpp"
#include "Core/Log.hpp"

#include "WaloPlayer.hpp"

BOOL     m_Active = FALSE;
float    m_FrameInterval = 1.0f;
id       m_DisplayLink = nil;
CGame*   m_GameInstance = nil;

CFTimeInterval m_Timestamp = 0.0;

@implementation AppDelegate

@synthesize Window;
@synthesize View;
@synthesize Controller;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    m_Active = FALSE;
    
    CGame* game = CPlayer::Instance();
    
    if(!game)
    {
        LOG("WaloEngine Critical: Game instance not created.\n");
        return FALSE;
    }
    
    Controller = [[CGameViewController alloc] init];
    
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    
    float deviceVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    
    if(deviceVersion < 8.0)
    {
        if(frame.size.width < frame.size.height)
        {
            frame = CGRectMake(frame.origin.y, frame.origin.x, frame.size.height, frame.size.width);
        }
    }
    
    Window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] applicationFrame]];
    
    //Window.backgroundColor = [UIColor redColor];
    
    View = [[CGameView alloc] initWithFrame: frame];
    
    //View.backgroundColor = [UIColor blueColor];
    //View.alpha = 0.5f;
    
    [Controller setView: self.View];
    
    [Window setRootViewController: self.Controller];
    
    [Window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    if(!game->Init())
    {
        LOG("WaloPlayer: Game initialization failed.\n");
        system("pause");
        return 0;
    }
    
    m_GameInstance = CGame::Instance();
    
    if(!m_GameInstance)
    {
        LOG("Critical Error: Game instance not created before app launch.\n");
        return NO;
    }
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    if(m_GameInstance)
    {
        [self StopUpdate];
        m_GameInstance->Activate(false);
    }
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{

}

- (void)applicationWillEnterForeground:(UIApplication *)application
{

}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if(m_GameInstance)
    {
        m_GameInstance->Activate(true);
        [self StartUpdate];
    }
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    if(m_GameInstance)
    {
        [self StopUpdate];
        
        m_GameInstance->Free();
        
        delete m_GameInstance;
    }
}

- (void)StartUpdate
{
    if(!m_Active)
    {
        m_DisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(Update:)];
        [m_DisplayLink setFrameInterval:m_FrameInterval];
        [m_DisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        m_Timestamp = ((CADisplayLink*)m_DisplayLink).timestamp;
        
        m_Active = true;
    }
}

- (void)StopUpdate
{
    if(m_Active)
    {
        m_Active = FALSE;
        
        [m_DisplayLink invalidate];
        m_DisplayLink = nil;
    }
}

- (void)Update:(id)sender
{
    CFTimeInterval dt = ((CADisplayLink*)sender).timestamp - m_Timestamp;
    m_Timestamp = ((CADisplayLink*)sender).timestamp;
    
    //Low FPS limit to ~30 FPS
    if(dt > 0.04)
        dt = 0.04;
    
    if(m_GameInstance)
    {
        [View preRender];
        
        m_GameInstance->Run((f32)dt);
        
        [View postRender];
    }
}

@end
