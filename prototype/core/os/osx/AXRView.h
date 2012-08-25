/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#import <Cocoa/Cocoa.h>

/**
 *  @brief (OSX only) This is a NSView subclass, for using inside a window in
 *  Apple's cocoa framework.
 */
@interface AXRView : NSView
{
    @private
    //this is actually always AXR::AXRWrapper *, but we hide it
    //so that we don't need to include the c++ header, which would
    //create the requirement to rename main.m to main.mm
    void * axrWrapper;
    BOOL needsFile;
}

@property(assign) BOOL needsFile;
/**
 *  Hack to make it work with IB from a dependent target.
 */
+(void) _keepAtLinkTime;

/**
 *  @return YES. This is for optimizing the drawing.
 */
-(BOOL) isOpaque;
/**
 *  Method that is called to draw on OSX. From within this, we can get access to the current
 *  graphics port, which is turned into a cairo surface.
 */
-(void) drawRect : (NSRect) dirtyRect;
/**
 *  @return YES. This allows the view to recieve events from the system.
 */
-(BOOL) acceptsFirstResponder;

/**
 *  Setter for the pointer to the wrapper.
 *  @param  theWrapper  A regular pointer to the wrapper object (but passed as a void *,
 *  to avoid including the c++ header).
 */
-(void) setAxrWrapper : (void *) theWrapper;
/**
 *  Getter for the pointer to the wrapper.
 *  @return A void * pointer to the wrapper object. Cast this to (AXR::CocoaAXRWrapper *).
 */
-(void *) axrWrapper;
/**
 *  Call this method to load a file. A file dialog will appear asking to select a file, and
 *  this is specific to each OS.
 *  @return A boolean telling wether the file has been loaded or not
 */
-(bool)loadFile;
/**
 *  Loads a XML or HSS file from the path you give.
 *  @param  filePath A NSString containing the path to the XML/HSS file.
 *  @return A boolean telling wether the file has been loaded or not
 */
-(bool)loadFile : (NSString *) filePath;
/**
 *  Reloads the currently loaded file.
 *  @return A boolean telling wether the file has been reloaded or not
 */
-(bool)reload;

@end
