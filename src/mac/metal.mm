// Dear ImGui: standalone example application for OSX + Metal.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#import <Foundation/Foundation.h>

#if TARGET_OS_OSX
#import <Cocoa/Cocoa.h>
#else
#import <UIKit/UIKit.h>
#endif

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include <res/fonts/default_font.h>

#include <lib/imgui/imgui.h>
#include <lib/imgui/mac/imgui_impl_metal.h>
#include <lib/imgui/mac/imgui_impl_osx.h>

#include "src/gui.h"

static s_gui gui;

@interface ViewController : NSViewController
@end

@interface ViewController () <MTKViewDelegate>
@property (nonatomic, readonly) MTKView *mtkView;
@property (nonatomic, strong) id <MTLDevice> device;
@property (nonatomic, strong) id <MTLCommandQueue> commandQueue;
@end

@implementation ViewController

- (instancetype)initWithNibName:(nullable NSString *)nibNameOrNil bundle:(nullable NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];

    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];

    if (!self.device)
    {
        NSLog(@"Metal is not supported");
        abort();
    }

    // Setup Dear ImGui context
    // FIXME: This example doesn't have proper cleanup...
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    // Must be before metal init
    // Because it initializes fonts and crashes if it is
    // initilized after metal
    gui_init(&gui, (s_gui_init) {});
    
    // Setup Renderer backend
    ImGui_ImplMetal_Init(_device);
    
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    
    return self;
}

- (MTKView *)mtkView
{
    return (MTKView *)self.view;
}

- (void)loadView
{
    self.view = [[MTKView alloc] initWithFrame:CGRectMake(0, 0, 1200, 720)];
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.mtkView.device = self.device;
    self.mtkView.delegate = self;

    // Add a tracking area in order to receive mouse events whenever the mouse is within the bounds of our view
    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:NSZeroRect
                                                                options:NSTrackingMouseMoved | NSTrackingInVisibleRect | NSTrackingActiveAlways
                                                                  owner:self
                                                               userInfo:nil];
    [self.view addTrackingArea:trackingArea];

    // If we want to receive key events, we either need to be in the responder chain of the key view,
    // or else we can install a local monitor. The consequence of this heavy-handed approach is that
    // we receive events for all controls, not just Dear ImGui widgets. If we had native controls in our
    // window, we'd want to be much more careful than just ingesting the complete event stream.
    // To match the behavior of other backends, we pass every event down to the OS.
    NSEventMask eventMask = NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged | NSEventTypeScrollWheel;
    [NSEvent addLocalMonitorForEventsMatchingMask:eventMask handler:^NSEvent * _Nullable(NSEvent *event)
    {
        ImGui_ImplOSX_HandleEvent(event, self.view);
        return event;
    }];

    ImGui_ImplOSX_Init();
}

#pragma mark - Interaction

- (void)mouseMoved:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)mouseDown:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)rightMouseDown:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)otherMouseDown:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)mouseUp:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)rightMouseUp:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)otherMouseUp:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)mouseDragged:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)rightMouseDragged:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)otherMouseDragged:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

- (void)scrollWheel:(NSEvent *)event {
    ImGui_ImplOSX_HandleEvent(event, self.view);
}

#pragma mark - MTKViewDelegate

- (void)drawInMTKView:(MTKView*)view
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = view.bounds.size.width;
    io.DisplaySize.y = view.bounds.size.height;

    CGFloat framebufferScale = view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;

    io.DisplayFramebufferScale = ImVec2(framebufferScale, framebufferScale);

    io.DeltaTime = 1 / float(view.preferredFramesPerSecond ?: 60);

    id<MTLCommandBuffer> commandBuffer = [self.commandQueue commandBuffer];
    
    static float clear_color[4] = { 0.28f, 0.36f, 0.5f, 1.0f };

    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;
    if (renderPassDescriptor != nil)
    {
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);

        // Here, you could do additional rendering work, including other passes as necessary.

        id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        [renderEncoder pushDebugGroup:@"ImGui demo"];

        // Start the Dear ImGui frame
        ImGui_ImplMetal_NewFrame(renderPassDescriptor);
        ImGui_ImplOSX_NewFrame(view);

        ImGui::NewFrame();

        gui.w_width = self.view.frame.size.width;
        gui.w_height = self.view.frame.size.height;
        gui_main(&gui);
        
//        CGRect newFrame = CGRectMake( self.view.window.frame.origin.x, self.view.window.frame.origin.y,
//                                     gui.w_width, gui.w_height);
//        [self.view.window setFrame:newFrame display:true];
        
        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        ImGui_ImplMetal_RenderDrawData(draw_data, commandBuffer, renderEncoder);

        [renderEncoder popDebugGroup];
        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:view.currentDrawable];
    }

    [commandBuffer commit];
}

- (void)mtkView:(MTKView*)view drawableSizeWillChange:(CGSize)size
{
}

@end

#pragma mark - Application Delegate

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, strong) NSWindow *window;
@end

@implementation AppDelegate

- (instancetype)init
{
    if (self = [super init])
    {
        NSViewController *rootViewController = [[ViewController alloc] initWithNibName:nil bundle:nil];
        self.window = [[NSWindow alloc] initWithContentRect:NSZeroRect
                                                  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                                    backing:NSBackingStoreBuffered
                                                      defer:NO];
        self.window.contentViewController = rootViewController;
        [self.window orderFront:self];
        [self.window center];
        [self.window becomeKeyWindow];
    }
    return self;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end

#pragma mark - main()

int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}

