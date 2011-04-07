//
//  SDL_Test2AppDelegate.h
//  SDL Test2
//
//  Created by Miro Keller on 03/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface SDL_Test2AppDelegate : NSObject <NSApplicationDelegate> {
@private
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
