//
//  main.m
//  mac
//
//  Created by Miro Keller on 13/02/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "../core/os/osx/AXRView.h"

int main(int argc, char *argv[])
{
    //hack to make it work with IB from a dependent target
    //if anyone knows how to avoid this, please tell me
    [AXRView _keepAtLinkTime];
    return NSApplicationMain(argc,  (const char **) argv);
}
