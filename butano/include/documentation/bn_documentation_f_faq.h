/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_FAQ_H
#define BN_DOCUMENTATION_FAQ_H

/**
 * @page faq Frequently asked questions (FAQ)
 *
 * @tableofcontents
 *
 *
 * @section faq_general General
 *
 *
 * @subsection faq_how_do_i How do I-
 *
 * The best way to get started is to read the
 * @ref getting_started "guide to download, install and start using Butano".
 *
 * After that, there are various @ref examples "examples" explaining most aspects of the engine.
 *
 * At last, check how to @ref import "import" your own assets in a game and
 * take a look at the <a href="modules.html">modules</a> page.
 *
 * This page is worth a look too.
 *
 *
 * @subsection faq_sell_money Would I be able to sell my game made with Butano for money?
 *
 * Sure!
 *
 * If you comply with <a href="https://github.com/GValiente/butano/blob/master/LICENSE">the Butano license</a>
 * and <a href="https://github.com/GValiente/butano/blob/master/licenses">the licenses of the third party libraries</a>
 * used by Butano, you can sell your game without issues.
 *
 * If you use assets such as the sprite font from the @ref examples "examples" or from the `common` folder,
 * you should also comply with their license. The licenses for these assets can be found in
 * <a href="https://github.com/GValiente/butano/blob/master/credits">the credits folder</a>.
 *
 *
 * @subsection faq_multiboot Is it possible to use Butano to create multiboot games?
 *
 * It is possible, but not recommended as Butano footprint is too big for multiboot stuff,
 * unless you want to make something *very* simple.
 *
 * Multiboot ROMs *should be* generated when they have the suffix `_mb`:
 *
 * `TARGET := $(notdir $(CURDIR))_mb`
 *
 *
 * @section faq_programming Programming
 *
 *
 * @subsection faq_cpp Can I use Butano without knowing C++?
 *
 * While Butano is easy to use if you have some C++ skills,
 * it is going to be hard to work with it if you don't know nothing about C++,
 * so you should learn a bit of C++ before diving into Butano.
 *
 * After that, maybe you can learn both C++ and Butano at the same time while you make a game,
 * but if you don't want to have a hard time with Butano you should know the basics of C++ before messing with it.
 *
 *
 * @subsection faq_ide Is there any good free IDE for Butano projects out there?
 *
 * Take a look at the @ref qt_creator.
 *
 *
 * @subsection faq_incomplete_type Why I get an incomplete type error when trying to use a Butano class?
 *
 * If you get an error message like these:
 *
 * ```
 * error: variable 'bn::regular_bg_ptr bg' has initializer but incomplete type
 * error: invalid use of incomplete type 'class bn::regular_bg_ptr'
 * error: aggregate 'bn::fixed' has incomplete type and cannot be defined
 * error: static assertion failed: template argument must be a complete class or an unbounded array
 * ```
 *
 * It is almost always because of lack of included headers.
 *
 * You can find the header of each Butano class in its documentation page.
 *
 *
 * @subsection faq_standard Why can't I use some standard functions and classes?
 *
 * You need to link the standard system libraries to use standard functions like `strlen`
 * and standard classes like `std::shared_ptr`.
 *
 * To do that, the `DEFAULTLIBS` variable of the `Makefile` of your project must not be empty. For example:
 *
 * `DEFAULTLIBS := true`
 *
 * Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).
 *
 * Since you are now linking the standard system libraries, you must comply with their license.
 * Also, don't expect all standard functions and classes to work after linking the standard system libraries.
 *
 *
 * @subsection faq_common_folder Why can't I use the content of the common folder in a new project?
 *
 * The `common` folder contains code and assets used by most Butano examples and tests.
 *
 * It is not part of Butano (the library), so it is not included by default in new projects.
 *
 * As the content of the `common` folder could change in future Butano versions,
 * you should copy its content to your project if you want to use it:
 * * Copy `common` `*.cpp` files to the `src` folder of your project.
 * * Copy `common` `*.h` files to the `include` folder of your project.
 * * Do the same with the rest of the files in the `common` folder (`*.bmp`, `*.json`, etc).
 *
 *
 * @subsection faq_subfolder Can I use subfolders for code and assets?
 *
 * Yes, but you need to add them to the `Makefile` of your project.
 *
 * For example:
 *
 * `SOURCES := src src/subfolder`
 *
 *
 * @subsection faq_stack_trace What function or method call is the source of an error message?
 *
 * You can enable stack trace logging in the `Makefile` of your project, so that when an error message is displayed,
 * the current stack trace will be logged.
 *
 * To enable stack trace logging, the `STACKTRACE` variable must not be empty. For example:
 *
 * `STACKTRACE := true`
 *
 * Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).
 *
 * If everything went well, the error message should indicate that the stack trace has been logged.
 * In that case, open the logs window of your emulator and the mangled stack trace should be there.
 * To demangle it, you can use <a href="http://demangler.com/">demangler.com</a> for example.
 *
 *
 * @subsection faq_error_after_update Why I get undefined reference errors after updating Butano?
 *
 * If after updating Butano you get error messages like this one:
 *
 * `error: undefined reference to 'bn::reciprocal_lut_16'`
 *
 * They usually go away with a full rebuild of your project (`make clean` before `make`).
 *
 * Also make sure you update <a href="https://devkitpro.org/">devkitARM</a> when you update Butano,
 * since some Butano features don't work with older devkitARM releases.
 *
 *
 * @subsection faq_destroy_ptr How to destroy sprites and backgrounds?
 *
 * bn::sprite_ptr, bn::regular_bg_ptr and all Butano classes that end with the `_ptr` suffix are
 * `std::shared_ptr` like smart pointers that retains shared ownership of a hardware resource.
 *
 * In the case of bn::sprite_ptr, several bn::sprite_ptr objects may own the same sprite.
 * The sprite is released when the last remaining bn::sprite_ptr owning it is destroyed.
 *
 * If you want to learn more about `std::shared_ptr`, you can read:
 * * <a href="https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-2">A basic introduction of standard smart pointers</a>.
 * * <a href="https://docs.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-shared-ptr-instances?view=msvc-160">A std::shared_ptr usage guide</a>.
 *
 *
 * @subsection faq_destroy_optional That's cool, but how can I destroy and reload sprites and backgrounds as easy as possible?
 *
 * bn::optional helps to unload and reload hardware resources easily:
 *
 * @code{.cpp}
 * // bg_optional is empty:
 * bn::optional<bn::regular_bg_ptr> bg_optional;
 *
 * // bg_optional now contains the background specified by bn::regular_bg_items::bg1:
 * bg_optional = bn::regular_bg_items::bg1.create_bg(0, 0);
 *
 * // bg_optional is empty again:
 * bg_optional.reset();
 *
 * // bg_optional now contains the background specified by bn::regular_bg_items::bg2:
 * bg_optional = bn::regular_bg_items::bg2.create_bg(0, 0);
 * @endcode
 *
 *
 * @subsection faq_float_double Can I use float or double when coding for the GBA?
 *
 * Modern CPUs have dedicated hardware for processing `float`s and `double`s.
 * The GBA doesn't have it, so you're left with two choices:
 *
 * * Using `float`s and `double`s emulated in software (*very* slow).
 * * Using fixed point arithmetic, which is just integer arithmetic with a few extra steps.
 *
 * Butano provides bn::fixed, which tries to be as close as possible to a `float` with fixed point arithmetic.
 * Use it when you want to work with numbers with decimal points.
 *
 *
 * @subsection faq_sprintf How do I print a number in a string?
 *
 * *Don't* use `sprintf`, `std::ostringstream` or anything else from the standard library, because:
 *
 * * Butano functions for printing numbers in strings can be more than 5 times faster than the standard functions.
 * * Standard functions can take more than 4KB of IWRAM.
 * * Standard functions sometimes can't be used because they throw link errors.
 *
 * If you want to print a number in a string with Butano, bn::to_string provides the easiest way to do it:
 *
 * @code{.cpp}
 * bn::string<16> string = bn::to_string<16>(number);
 * @endcode
 *
 * If you want to compose a string with multiple numbers, you can use bn::ostringstream too:
 *
 * @code{.cpp}
 * bn::string<64> string;
 * bn::ostringstream string_stream(string);
 * string_stream << "Text: ";
 * string_stream << 123;
 * string_stream << " - ";
 * string_stream << number;
 * @endcode
 *
 *
 * @subsection faq_random_seed How can I set the seed of a bn::random?
 *
 * If you want to do that, use bn::seed_random instead.
 *
 *
 * @subsection faq_rect_odd_dimensions Does bn::rect work with odd dimensions?
 *
 * Since it takes a center point as position instead of a top-left point, bn::rect has problems with odd dimensions.
 *
 * If you need to handle odd dimensions, use bn::top_left_rect instead.
 *
 *
 * @subsection faq_delta_time How can I get the delta time?
 *
 * You can usually assume than your game is running at 60FPS, so to wait one second
 * you should call bn::core::update 60 times. In this case, the delta time (the time elapsed
 * between two bn::core::update calls) is always one frame, or `1 / 60` seconds.
 *
 * However, if your game is so choppy that it has variable frame rate (it shouldn't), you can get
 * the number of frames that were missed in the last bn::core::update call with bn::core::last_missed_frames.
 * The delta time is the number of missed frames plus one:
 *
 * @code{.cpp}
 * int elapsed_frames = bn::core::last_missed_frames() + 1;
 * @endcode
 *
 *
 * @subsection faq_wait_updates Is there a way to stop running my code for a certain amount of time?
 *
 * Since you can usually assume than your game is running at 60FPS,
 * for example you can wait one second with this code:
 *
 * @code{.cpp}
 * for(int index = 0; index < 60; ++index)
 * {
 *     bn::core::update();
 * }
 * @endcode
 *
 *
 * @subsection faq_code_crash How can I know what code makes my game crash?
 *
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> exception system allows to catch common programming errors.
 * When an exception is triggered, No$gba can stop the execution of the ROM and show the code that has triggered the exception.
 * Please check the @ref nocashgba_exception if you want to try it.
 *
 * As always, you also can remove code until the crash goes away.
 *
 *
 * @subsection faq_tonc_general_notes Are there some more general notes on GBA programming out there?
 *
 * <a href="https://gbadev.net/tonc/first.html#sec-notes">I'm glad you asked</a>.
 *
 *
 * @section faq_memory Memory
 *
 *
 * @subsection faq_memory_containers Why are there std like containers included with Butano?
 *
 * Butano containers differ from the standard library ones in two important points:
 * * They don't use the heap, their content is always on the stack.
 * * They don't throw exceptions. @ref assert are used instead.
 *
 * Since avoiding heap usage and exceptions is usually good for GBA development,
 * use Butano containers whenever possible.
 *
 * Keep in mind that unlike most Butano containers, bn::unique_ptr uses the heap instead of the stack.
 *
 *
 * @subsection faq_memory_heap So I shouldn't use the heap?
 *
 * Since heap usage is slow and the heap allocator included with Butano is not very fast,
 * avoid heap usage whenever possible.
 *
 * Also, remember to call bn::core::init before using the heap.
 *
 *
 * @subsection faq_memory_types Why I run out of memory so often?
 *
 * Besides VRAM and such, the GBA provides two memory banks:
 * * IWRAM: 32KB fast RAM.
 * * EWRAM: 256KB slow RAM.
 *
 * Data is allocated in IWRAM by default, so it is common to run out of memory if you don't use EWRAM.
 *
 * To place data in EWRAM, you can:
 * * Allocate memory in the heap, since it is placed in EWRAM.
 * * Declare static data with the `BN_DATA_EWRAM` macro:
 * @code{.cpp}
 * BN_DATA_EWRAM static_data data;
 * @endcode
 *
 * However, if the data is read only, you can avoid wasting RAM by placing it in ROM with the `constexpr` qualifier:
 * @code{.cpp}
 * constexpr const_data data;
 * @endcode
 *
 * bn::memory provides functions to query available and used RAM, like bn::memory::used_stack_iwram
 * and bn::memory::used_static_iwram.
 *
 * To avoid running out of IWRAM, Butano Fighter and Varooom 3D place all scenes in EWRAM.
 * Check their `main.cpp` files to see how it works.
 *
 *
 * @subsection faq_memory_global_objects Does Butano allow to declare bn::sprite_ptr or bn::regular_bg_ptr objects globally?
 *
 * In general, you should not do anything with Butano before calling bn::core::init,
 * including creating global Butano objects and allocating memory in the heap.
 *
 * If you want to declare global Butano objects, you can do something like this instead:
 *
 * @code{.cpp}
 * struct global_data
 * {
 *     bn::sprite_ptr sprite;
 *     bn::regular_bg_ptr bg;
 * };
 *
 * global_data* global_ptr;
 *
 * int main()
 * {
 *     bn::core::init();
 *
 *     global_data global_instance = {
 *         bn::sprite_items::sprite_item.create_sprite(0, 0),
 *         bn::regular_bg_items::bg_item.create_bg(0, 0)
 *     };
 *
 *     global_ptr = &global_instance;
 *
 *     // ...
 * }
 * @endcode
 *
 * With that, you can access global Butano objects from anywhere in your project with this code:
 *
 * @code{.cpp}
 * global_ptr->sprite.set_position(50, 50);
 * @endcode
 *
 *
 * @subsection faq_memory_arm_iwram How can I generate ARM code in IWRAM?
 *
 * By default, functions and methods are compiled to Thumb code and placed in ROM.
 * If you want to increase the performance of a function/method, a good way is to compile it to ARM code
 * and place it in IWRAM.
 *
 * To do it, you have to:
 * * Place the `BN_CODE_IWRAM` macro before the function/method declaration to indicate its section.
 * For example:
 * @code{.cpp}
 * BN_CODE_IWRAM void my_function(int arg);
 * @endcode
 * * Place the function/method definition in a file with extension `.bn_iwram.cpp`.
 *
 * For example, the `world_map` example generates ARM code in IWRAM for the `load_attributes` function.
 *
 * Keep in mind that IWRAM is small, so you shouldn't place too much code in it.
 *
 *
 * @section faq_images Images
 *
 *
 * @subsection faq_images_invalid_header_size Why I get an "invalid header size" error when I try to import a *.bmp file?
 *
 * If you get that error when importing a `*.bmp` file, it probably means that it contains unsupported features, such as:
 * * Compression.
 * * Color space information.
 * * Non-indexed colors.
 *
 * Please check the @ref import_image import guide to learn more about how to generate supported `*.bmp` files.
 *
 *
 * @section faq_color Colors
 *
 *
 * @subsection faq_color_transparent Which color is the transparent one?
 *
 * Butano supports 16 or 256 color images only, so they must have a color palette.
 *
 * The transparent color is the first one in the color palette,
 * so in order to change it you should use a bitmap editor with color palette manipulation tools,
 * like <a href="https://github.com/gb-archive/usenti">Usenti</a>:
 *
 * @image html import_usenti.png
 *
 *
 * @subsection faq_color_backdrop How can I set the backdrop color?
 *
 * The transparent or the backdrop color (displayed color when nothing else is)
 * is the first one in the backgrounds palette.
 *
 * You can override its default value with bn::bg_palettes::set_transparent_color.
 *
 *
 * @subsection faq_color_share_palettes How to share the same color palette between sprites or backgrounds?
 *
 * If two sprites or backgrounds have the same colors, by default they share the same color palette.
 *
 * Keep in mind that unused colors are also taken into account when deciding if two color palettes are equal or not.
 *
 *
 * @subsection faq_color_8bpp_palette_change Why changing the color palette of an 8BPP sprite or background doesn't work?
 *
 * Since the GBA has only 256 colors for sprites, if you use two sprites with more than 16 colors at the same time,
 * Butano assumes that they have the same color palette (same colors in the same order).
 *
 * So if you change the palette of an 8BPP sprite with a new palette item, Butano assumes that the palette
 * created with the palette item is the same as the old one and doesn't update the colors of the sprite:
 *
 * @code{.cpp}
 * sprite.set_palette(palette_item);
 * @endcode
 *
 * If what you want to do is to update the colors of the sprite, you have to retrieve the palette of the sprite
 * and update its colors:
 *
 * @code{.cpp}
 * bn::sprite_palette_ptr sprite_palette = sprite.palette();
 * sprite_palette.set_colors(palette_item);
 * @endcode
 *
 * The same happens with 8BPP backgrounds.
 *
 *
 * @section faq_sprites Sprites
 *
 *
 * @subsection faq_sprites_top_left Why sprite coordinates are relative to the center of the screen, instead of to its top-left corner?
 *
 * If you don't like it, you can always use the `top_left` methods to specify coordinates relative
 * to the top-left corner of the screen. They're a bit slower than the regular ones, though.
 *
 *
 * @subsection faq_sprites_multiple_8bpp Why everything looks weird when I show two or more sprites with more than 16 colors?
 *
 * Since the GBA has only 256 colors for sprites, if you use two sprites with more than 16 colors at the same time,
 * Butano assumes that they have the same color palette (same colors in the same order).
 *
 * So if you are going to show multiple sprites with more than 16 colors at the same time, use the same color palette
 * with all of them (in the same scene of course, sprites shown in different scenes can have different color palettes).
 *
 *
 * @subsection faq_sprites_item_from_ptr Is there a way to get the bn::sprite_item used to create a bn::sprite_ptr?
 *
 * No :)
 *
 *
 * @subsection faq_sprites_scanline Why whenever I have too many sprites on screen, some of them get cut off?
 *
 * If you show too many sprites on screen (especially if they're rotated or scaled),
 * they won't be rendered because the GBA has a
 * <a href="https://problemkaputt.de/gbatek-lcd-obj-overview.htm">maximum number of sprite pixels per scanline</a>.
 *
 * You can try to get around this limitation by showing sprites without rotation or scaling, or even better,
 * using backgrounds instead of sprites.
 *
 *
 * @subsection faq_sprites_metasprites Does Butano support metasprites?
 *
 * Metasprites (groups of regular sprites that together represent one single bigger sprite) are not supported by Butano.
 *
 *
 * @subsection faq_sprites_hidden Does hiding a sprite make it count towards the total number of allowed sprites?
 *
 * Hidden sprites are not committed to the GBA, but they still take resources like color palettes and VRAM.
 *
 * The easiest way to increase the maximum number of hidden sprites allowed for a specific project is
 * to define it in the `USERFLAGS` of its `Makefile`.
 *
 * For example, to set the maximum number of hidden sprites to 256:
 *
 * `USERFLAGS := -DBN_CFG_SPRITES_MAX_ITEMS=256`
 *
 * Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).
 *
 *
 * @subsection faq_sprites_utf8_characters How can I print UTF-8 characters like Japanese or Chinese ones?
 *
 * bn::sprite_text_generator already supports UTF-8 characters rendering,
 * but the bn::sprite_font instances used in the @ref examples "examples" don't provide Japanese or Chinese characters,
 * so you will have to make a new one with them.
 *
 *
 * @section faq_backgrounds Backgrounds
 *
 *
 * @subsection faq_backgrounds_top_left Why background coordinates are relative to the center of the screen, instead of to its top-left corner?
 *
 * If you don't like it, you can always use the `top_left` methods to specify coordinates relative
 * to the top-left corner of the screen. They're a bit slower than the regular ones, though.
 *
 *
 * @subsection faq_backgrounds_multiple_8bpp Why everything looks weird when I show two or more backgrounds with more than 16 colors?
 *
 * Since the GBA has only 256 colors for tiled backgrounds, if you use two 8BPP backgrounds,
 * Butano assumes that they have the same color palette (same colors in the same order).
 *
 * So if you are going to show multiple 8BPP backgrounds, you can:
 * * Use the same color palette with all of them (in the same scene of course, backgrounds shown in different scenes
 *   can have different color palettes).
 * * Change their BPP mode to 4BPP, so each background can have its own color palette with more than 16 colors.
 *
 *
 * @subsection faq_backgrounds_big What's a big background?
 *
 * The GBA only supports some fixed sizes for background maps.
 *
 * However, Butano allows to manage background maps with any size up to 16384 pixels and multiple of 256 pixels.
 * These special background maps and the backgrounds that display them are called big maps/backgrounds.
 *
 * Try to avoid big backgrounds whenever possible, because they are slower CPU wise.
 *
 *
 * @subsection faq_backgrounds_regular_affine Why there are two types of backgrounds (regular and affine)?
 *
 * It seems it is always better to use affine backgrounds, since they can be rotated, scaled, etc.
 * and its size can be up to 1024x1024 pixels without becoming big backgrounds.
 *
 * However, compared to regular backgrounds, affine backgrounds have these limitations:
 * * Only two of them can be displayed at the same time, instead of four.
 * * They don't support 16 color tiles, only 256 color ones.
 * * They only support up to 256 different tiles, instead of 1024.
 * * They don't support flipped tiles.
 *
 * Because of these limitations, you should avoid affine backgrounds whenever possible.
 *
 *
 * @subsection faq_backgrounds_animated How can I show a background with animated tiles?
 *
 * There are two ways, the easy one and the powerful one:
 * * If you only want to show some basic tile animation, such the water or the grass tiles in top-down RPGs,
 *   you can use one of these actions: bn::regular_bg_animate_action, bn::regular_bg_cached_animate_action,
 *   bn::affine_bg_animate_action and bn::affine_bg_cached_animate_action.
 *   The `regular_bgs` and `affine_bgs` examples show how to use some of them.
 * * If you want to show something more advanced, you need to manage the background map by yourself.
 *   The `dynamic_regular_bg` and `dynamic_affine_bg` examples show how to do it.
 *
 *
 * @subsection faq_backgrounds_error_grit Why can't I import a regular background with 1024 or less tiles?
 *
 * If you get this error when trying to import a regular background with 1024 or less tiles:
 *
 * `error: Regular BGs with more than 1024 tiles not supported: 1025`
 *
 * Or you get this error when importing an affine background with 256 or less tiles:
 *
 * `error: Affine BGs with more than 256 tiles not supported: 257`
 *
 * Your image is fine, but <a href="https://github.com/devkitPro/grit">grit</a>
 * (the tool used by Butano to import images) is generating unneeded extra tiles.
 *
 * The only workaround that I know of is reducing detail in your input image until the tiles count of
 * the generated background is valid.
 *
 * However, if you are working with unique tiles to generate a dynamic background,
 * importing background tiles only instead of a background with map should work.
 * Check @ref import_regular_bg_tiles, @ref import_affine_bg_tiles, the `dynamic_regular_bg` example and
 * the `dynamic_affine_bg` example for that.
 *
 *
 * @subsection faq_backgrounds_hidden Does hiding a background make it count towards the total number of allowed backgrounds?
 *
 * Hidden backgrounds are not committed to the GBA, but they still take resources like color palettes and VRAM.
 *
 * The easiest way to increase the maximum number of hidden backgrounds allowed for a specific project is
 * to define it in the `USERFLAGS` of its `Makefile`.
 *
 * For example, to set the maximum number of hidden backgrounds to 8:
 *
 * `USERFLAGS := -DBN_CFG_BGS_MAX_ITEMS=8`
 *
 * Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).
 *
 *
 * @section faq_audio Audio
 *
 *
 * @subsection faq_audio_music_crash Why the game crashes when some Direct Sound songs are played?
 *
 * Butano uses the excellent <a href="https://maxmod.devkitpro.org/">Maxmod</a> library
 * for Direct Sound audio support.
 *
 * It provides impressive performance and support for lots of module music formats,
 * but unfortunately it crashes with some songs.
 *
 * Sometimes it helps to change song's file format (for example, from `*.xm` to `*.it`).
 * You can use <a href="https://openmpt.org/">OpenMPT</a> to do that.
 *
 * You can also try to create a new issue in its
 * <a href="https://github.com/devkitPro/maxmod/issues">GitHub issues</a> page,
 * but since it seems the library was abandoned long time ago, don't get your hopes up too high.
 *
 *
 * @subsection faq_audio_music_wav Why can't I use a long *.wav file as music?
 *
 * <a href="https://maxmod.devkitpro.org/">Maxmod</a> doesn't allow to play long `*.wav` files as music,
 * unfortunately.
 *
 * The maximum length of sound samples is also small, so if you try to play a long `*.wav` file as a sound effect,
 * it should stop after a few seconds.
 *
 * What you can do is to split your long `*.wav` file in small pieces and put them in a module file
 * (files with `*.mod`, `*.xm`, `*.s3m` and `*.it` extensions).
 * You can use <a href="https://sox.sourceforge.net/">SoX</a> to split the long `*.wav` file and
 * <a href="https://openmpt.org/">OpenMPT</a> to create the final module file.
 *
 * You also can use <a href="https://github.com/Fralacticus/gba-wav-to-s3m-converter">GBA Wav to S3m Converter</a>,
 * a tool that generates a `*.s3m` file from a long `*.wav` automatically.
 *
 *
 * @subsection faq_audio_music_missing_notes Why there are missing notes when playing some Direct Sound songs?
 *
 * If a song doesn't have more channels than the maximum number of active Direct Sound music channels
 * specified by @ref BN_CFG_AUDIO_MAX_MUSIC_CHANNELS, as before,
 * sometimes it helps to change its file format (for example, from `*.xm` to `*.it`).
 * You can use <a href="https://openmpt.org/">OpenMPT</a> to do that.
 *
 *
 * @subsection faq_audio_quality How can I improve Direct Sound audio quality?
 *
 * If you have some free CPU left, you can increase Direct Sound audio mixing rate to improve its quality.
 *
 * The easiest way to specify Direct Sound audio mixing rate for a specific project is
 * to define it in the `USERFLAGS` of its `Makefile`.
 *
 * For example, to set Direct Sound audio mixing rate to 21KHz:
 *
 * `USERFLAGS := -DBN_CFG_AUDIO_MIXING_RATE=BN_AUDIO_MIXING_RATE_21_KHZ`
 *
 * Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).
 *
 * Available Direct Sound mixing rates are @ref audio "here".
 *
 *
 * @section faq_flash_carts Flash carts
 *
 *
 * @subsection faq_flash_carts_sram Why my game runs fine on emulators but doesn't work on a real GBA with a flash cart?
 *
 * Emulators usually initialize SRAM with zeros, while some flash carts don't.
 * You should check if SRAM is formatted and format it if it isn't. The `sram` example shows how to do it.
 *
 * Also, some flash carts allow to improve commercial games with patches like `saver patch`, `enable restart`,
 * `enable real time save`, etc. These patches can break homebrew games, so try to disable some or all of them
 * if you run into any issues.
 *
 *
 * @subsection faq_flash_carts_sram_old Why SRAM works on emulators but doesn't work with this old flash cart?
 *
 * While SRAM works out-of-the-box with most modern flash carts, it can fail with some older ones.
 *
 * To fix it you can try to:
 * * Set save type as SRAM.
 * * Disable or enable save patches.
 * * Update the firmware of the flash cart.
 */

#endif
