#ifdef PLATFORM_MAC

#import "AppDelegate.h"
#import <MetalKit/MetalKit.h>
#import "RendererView.h"
#include "ApplicationPlatformWrapper.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)aNotification {
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end

#endif
