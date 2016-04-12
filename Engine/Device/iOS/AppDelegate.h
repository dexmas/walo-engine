#pragma once

#import <UIKit/UIKit.h>
#import "Device/iOS/GameView.h"
#import "Device/iOS/GameViewController.h"

@interface AppDelegate: UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow* Window;
@property (strong, nonatomic) CGameView* View;
@property (strong, nonatomic) CGameViewController* Controller;

@end

