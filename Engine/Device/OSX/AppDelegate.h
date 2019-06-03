#pragma once

#import <Cocoa/Cocoa.h>
#import "Device/OSX/GameView.h"

@interface AppDelegate: NSObject <NSApplicationDelegate, NSWindowDelegate>

@property (strong, nonatomic) NSWindow* Window;
@property (strong, nonatomic) CGameView* View;

@end

