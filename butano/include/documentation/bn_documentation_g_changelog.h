/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_CHANGELOG_H
#define BN_DOCUMENTATION_CHANGELOG_H

/**
 * @page changelog Changelog
 *
 * @tableofcontents
 *
 *
 * @section changelog_18_8_0 18.8.0 (next release)
 *
 * * bn::best_fit_allocator::check_empty_on_destructor and bn::best_fit_allocator::set_check_empty_on_destructor added.
 * * @ref BN_CFG_ASSERT_TAG added.
 * * Import tool now allows to specify the width of sprites and sprite tiles.
 *   See the @ref import_image import guide to learn how to specify it.
 * * Show the sum of the ticks of all entries in the profiler results.
 * * CPU usage reduced in most resource managers.
 * * @ref getting_started guide improved.
 *
 *
 * @section changelog_18_7_1 18.7.1
 *
 * * Placement `new` calls with user-provided `operator new` overloads fixed.
 * * bn::unordered_map and bn::unordered_set clangd warnings fixed.
 *
 *
 * @section changelog_18_7_0 18.7.0
 *
 * * bn::green_swap_mode added (thanks Sono!). Check the `green_swap` example to see how to set
 *   how a background must be displayed when green swap is enabled.
 * * bn::unique_ptr `operator<<` added.
 * * bn::ibitset `operator<<` added.
 * * Hash support for bn::ibitset added.
 * * More bn::istring_base comparison operators added.
 * * bn::utf8_character validation improved.
 * * Green swap disabled when the display is stopped.
 * * `libstdc++` is now linked by default (thanks asie!).
 * * @ref faq_delta_time, @ref faq_sprites_metasprites and @ref faq_backgrounds_animated questions
 *   added to the @ref faq page.
 * * @ref faq_standard and @ref faq_memory_heap questions improved.
 * * @ref getting_started_wt guide updated.
 * * @ref qt_creator improved.
 * * Some spelling mistakes fixed.
 *
 *
 * @section changelog_18_6_0 18.6.0
 *
 * * Big maps maximum size is limited to 16384 pixels.
 * * `create_new` map methods are deprecated.
 * * Small ROM usage reduction.
 * * bn::sprite_ptr::create_optional fixed.
 * * @ref faq_subfolder question added to the @ref faq page.
 *
 *
 * @section changelog_18_5_0 18.5.0
 *
 * * bn::sprite_text_generator uses one sprite per character if the space between characters is too big.
 * * bn::sprite_ptr::horizontal_shear and bn::sprite_ptr::vertical_shear fixed.
 * * bn::sprite_font space between characters validation fixed.
 * * @ref faq_memory_arm_iwram question added to the @ref faq page.
 *
 *
 * @section changelog_18_4_0 18.4.0
 *
 * * bn::random::get_bool and bn::seed_random::get_bool added.
 * * bn::bitset_ref move constructor added.
 * * @ref BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED and @ref BN_CFG_SPRITE_TILES_SANITY_CHECK_ENABLED added.
 * * bn::random::get_int and bn::seed_random::get_int optimized.
 * * BG blocks manager allocation fixed.
 * * bn::sprite_tiles::log_status items count fixed.
 *
 *
 * @section changelog_18_3_0 18.3.0
 *
 * * bn::sprite_text_generator::generate_top_left and bn::sprite_text_generator::generate_top_left_optional added.
 * * bn::sprite_text_generator allows to setup the mosaic, the blending, the visibility and the camera
 *   of the output sprites.
 * * bn::sprite_builder::release_affine_mat, bn::sprite_builder::release_camera,
 *   bn::regular_bg_builder::release_camera and bn::affine_bg_builder::release_camera fixed.
 * * bn::sprite_builder, bn::regular_bg_builder and bn::affine_bg_builder documentation improved.
 *
 *
 * @section changelog_18_2_1 18.2.1
 *
 * * bn::forward_list and bn::intrusive_forward_list forward iterator requirements fixed.
 * * bn::list, bn::intrusive_list, bn::unordered_map and bn::unordered_set bidirectional iterator requirements fixed.
 * * bn::deque iterator random access requirements fixed.
 * * bn::deque, bn::list, bn::intrusive_list, bn::forward_list, bn::intrusive_forward_list, bn::unordered_map
 *   and bn::unordered_set iterator type aliases fixed.
 *
 *
 * @section changelog_18_2_0 18.2.0
 *
 * * bn::regular_bg_ptr, bn::regular_bg_builder, bn::affine_bg_ptr, bn::affine_bg_builder,
 *   bn::affine_bg_mat_attributes, bn::sprite_ptr and bn::sprite_builder
 *   support top-left coordinates for their position.
 * * bn::regular_bg_top_left_move_to_action, bn::regular_bg_top_left_move_loop_action,
 *   bn::regular_bg_top_left_move_toggle_action, bn::affine_bg_top_left_move_to_action,
 *   bn::affine_bg_top_left_move_loop_action, bn::affine_bg_top_left_move_toggle_action,
 *   bn::sprite_top_left_move_to_action, bn::sprite_top_left_move_loop_action
 *   and bn::sprite_top_left_move_toggle_action added.
 * * bn::safe_degrees_angle, bn::degrees_lut_sin_safe, bn::degrees_lut_cos_safe
 *   and bn::degrees_lut_sin_and_cos_safe added.
 * * bn::affine_mat_attributes::set_rotation_angle_safe, bn::affine_bg_builder::set_rotation_angle_safe,
 *   bn::affine_bg_mat_attributes::set_rotation_angle_safe, bn::affine_bg_ptr::set_rotation_angle_safe,
 *   bn::sprite_affine_mat_ptr::set_rotation_angle_safe, bn::sprite_builder::set_rotation_angle_safe
 *   and bn::sprite_ptr::set_rotation_angle_safe added.
 * * Position in `create` methods is now optional.
 * * bn::intrusive_list and bn::intrusive_forward_list iterator constructors are now public.
 * * Real time clock (RTC) string added.
 * * SRAM string with link-time optimization fixed.
 * * @ref faq_backgrounds_top_left and @ref faq_sprites_top_left questions added to the @ref faq page.
 * * @ref faq_flash_carts_sram question improved.
 *
 *
 * @section changelog_18_1_0 18.1.0
 *
 * * bn::blending::set_transparency_and_intensity_alpha added.
 * * bn::timer::elapsed_ticks_with_restart added.
 * * bn::timers::ticks_per_second added.
 * * @ref BN_CFG_TIMER_FREQUENCY added.
 * * bn::timer overflow fixed.
 * * bn::degrees_lut_sin, bn::degrees_lut_cos and bn::degrees_lut_sin_and_cos optimized.
 * * Color palettes, sprite tiles and audio management ROM usage reduced.
 * * `timer` example added.
 * * @ref faq_sprites_item_from_ptr question added to the @ref faq page.
 * * <a href="https://github.com/chrislewisdev/gba-pseudo-3d">GBA pseudo-3D demo</a> added to `README.md`.
 *
 *
 * @section changelog_18_0_1 18.0.1
 *
 * Tools no longer crash on Windows with more than 60 logical CPU cores.
 *
 *
 * @section changelog_18_0_0 18.0.0
 *
 * * Language standard updated to C++23.
 * * Old bn::hdma::start and bn::hdma::high_priority_start API deprecated.
 * * `BN_UNREACHABLE` deprecated.
 * * Construction of bn::string and bn::string_view from `nullptr` is disallowed (thanks C++23).
 * * bn::string::starts_with, bn::string::ends_with, bn::string_view::starts_with and bn::string_view::ends_with
 *   a `nullptr` char array is disallowed.
 * * bn::unique_ptr is now `constexpr`.
 * * bn::bitset and bn::string aren't `constexpr` anymore (it didn't work before).
 * * More overloads added to bn::string construction, assignment, bn::string::assign and bn::string::append.
 * * bn::string::find and bn::string_view::find added.
 * * bn::string::contains and bn::string_view::contains added.
 * * bn::string::substr added.
 * * bn::string_view::ends_with a char array added.
 * * `nullptr` constructor and method overloads added to bn::unique_ptr.
 * * Non-throwing `new` and `delete` supported.
 * * bn::string_view::remove_prefix and bn::string_view::remove_suffix assert fixed.
 * * `bn::is_constant_evaluated` replaced with `consteval`.
 * * Unnecessary `operator!=` implementations removed.
 * * bn::regular_bg_builder::release_map, bn::affine_bg_builder::release_map,
 *   bn::sprite_builder::release_tiles and bn::sprite_builder::release_palette ROM usage reduced.
 * * bn::deque iterators order operators optimized.
 *
 *
 * @section changelog_17_9_0 17.9.0
 *
 * * bn::hdma::start and bn::hdma::high_priority_start API improved.
 * * Some assert messages improved.
 * * Missing header includes added.
 * * `hdma_palette` example added.
 * * Dead links updated.
 * * New games made with Butano added to `README.md`.
 *
 *
 * @section changelog_17_8_1 17.8.1
 *
 * * bn::deque::erase fixed (thanks <a href="https://github.com/copyrat90">copyrat90</a>!).
 * * @ref faq_sell_money question improved.
 *
 *
 * @section changelog_17_8_0 17.8.0
 *
 * * H-Blank effects stop fixed.
 * * `random` example added.
 * * `log` example improved.
 * * @ref nocashgba_exception added.
 * * @ref faq_code_crash question added to the @ref faq page.
 * * @ref BN_LOG_BACKEND_MGBA also works with <a href="https://github.com/SourMesen/Mesen2">Mesen</a>.
 * * bn::sram documentation improved.
 * * @ref import_regular_bg and @ref import_affine_bg import guides improved.
 *
 *
 * @section changelog_17_7_0 17.7.0
 *
 * * bn::bg_palette_ptr::set_color and bn::sprite_palette_ptr::set_color added.
 * * Empty bn::utf8_characters_map and bn::utf8_characters_map_ref allowed.
 *
 *
 * @section changelog_17_6_0 17.6.0
 *
 * * bn::bg_palette_ptr::rotate_range_start, bn::bg_palette_ptr::rotate_range_size
 *   and bn::bg_palette_ptr::set_rotate_range added.
 * * bn::sprite_palette_ptr::rotate_range_start, bn::sprite_palette_ptr::rotate_range_size
 *   and bn::sprite_palette_ptr::set_rotate_range added.
 * * bn::fixed::modulo added.
 * * bn::memory::fast_ewram added.
 * * bn::random::get_unbiased_int, bn::random::get_unbiased_fixed, bn::seed_random::get_unbiased_int
 *   and bn::seed_random::get_unbiased_fixed added (thanks <a href="https://github.com/alope107">alope107</a>!).
 * * H-Blank effects management optimized.
 * * @ref BN_EWRAM_WAIT_STATE_1 crash after boot documented.
 * * Slow EWRAM test added.
 * * IWRAM vs EWRAM stack tests added to the `profiler` coroutine test.
 * * Slow game pak test improved.
 * * GCC14 false build warnings in Butano Fighter fixed.
 *
 *
 * @section changelog_17_5_0 17.5.0
 *
 * * bn::memory::dma_enabled and bn::memory::set_dma_enabled added.
 * * bn::fixed explicit conversion operators added.
 * * Custom C++ flags can be specified in the project's `Makefile`.
 * * All logging functions and methods can be called with logging disabled.
 * * Current <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a> implementation
 *   has been moved to a private namespace to allow using other implementations.
 * * Spelling mistakes corrected.
 * * Background z order documentation improved.
 * * <a href="https://github.com/Fralacticus/gba-wav-to-s3m-converter">GBA Wav to S3m Converter</a>
 *   added to the @ref faq page.
 *
 *
 * @section changelog_17_4_0 17.4.0
 *
 * * bn::blending::set_black_fade_color and bn::blending::set_white_fade_color added.
 * * bn::fixed `friend` operators allow different levels of precision.
 * * bn::list and bn::forward_list unnecessary asserts removed.
 * * VRAM related error messages shortened.
 * * `profiler` coroutine test improved.
 * * BIOS calls can be disabled in the `profiler` test.
 * * @ref faq_standard question added to the @ref faq page.
 * * @ref faq_memory_types question improved.
 * * <a href="https://neofidstudios.itch.io/demons-of-asteborg-dx">Demons of Asteborg DX</a> added to `README.md`.
 *
 *
 * @section changelog_17_3_0 17.3.0
 *
 * * bn::core::current_cpu_ticks, bn::core::last_cpu_ticks and bn::core::last_vblank_ticks added.
 * * Import tool now allows to exclude Huffman when choosing the best compression for images.
 *   See the @ref import_image import guide to learn how to exclude it.
 * * LZ77 decompression optimized (thanks Lorenzooone!).
 * * bn::core::current_cpu_usage and bn::core::last_vblank_usage with skip frames fixed.
 * * Coroutine added to the `profiler` test.
 *
 *
 * @section changelog_17_2_0 17.2.0
 *
 * * bn::core::log_stacktrace added. Check the `log` example to see how to log the current stack trace.
 * * bn::isprite_animate_action::next_change_updates, bn::isprite_cached_animate_action::next_change_updates,
 *   bn::iregular_bg_animate_action::next_change_updates, bn::iregular_bg_cached_animate_action::next_change_updates,
 *   bn::iaffine_bg_animate_action::next_change_updates and bn::iaffine_bg_cached_animate_action::next_change_updates added.
 * * `bn::pair` is now a `std::pair` alias.
 * * <a href="https://www.coranac.com/man/tonclib/">Tonclib</a> ASM ROM usage reduced.
 * * `profiler` example simplified.
 * * Compression algorithms added to the `profiler` test.
 * * @ref faq_stack_trace question added to the @ref faq page.
 * * @ref faq_color_8bpp_palette_change question added to the @ref faq page.
 * * @ref faq_sprites_hidden and @ref faq_backgrounds_hidden questions added to the @ref faq page.
 * * @ref getting_started guide improved.
 * * @ref faq_memory_types question improved.
 * * <a href="https://github.com/nikku4211/wavefront2varooom3d">Wavefront To Varooom 3D</a> added to `README.md`.
 *
 *
 * @section changelog_17_1_1 17.1.1
 *
 * bn::unique_ptr move fixed.
 *
 *
 * @section changelog_17_1_0 17.1.0
 *
 * * Affine big maps canvas size can be specified by the user: bn::affine_bg_big_map_canvas_size,
 *   bn::bg_maps::new_affine_big_map_canvas_size, bn::bg_maps::set_new_affine_big_map_canvas_size and
 *   bn::affine_bg_map_ptr::big_canvas_size added.
 * * Regular big maps canvas height increased.
 * * Force at least one update before sleeping in bn::core::sleep.
 * * bn::span::size_bytes implicit conversion warning fixed.
 * * <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a> binary files build time reduced.
 * * Scale can be changed in the `world_map` example.
 *
 *
 * @section changelog_17_0_0 17.0.0
 *
 * * Regular and affine backgrounds allow to specify if they are part of the blending bottom layer or not.
 *   See the `blending` example to learn how to do it.
 * * bn::sprites::blending_bottom_enabled and bn::sprites::set_blending_bottom_enabled added.
 * * bn::backdrop functions and actions added.
 * * bn::backdrop_color_hbe_ptr added.
 * * bn::fixed_point_t, bn::fixed_size_t, bn::fixed_rect_t and bn::top_left_fixed_rect_t added.
 * * Collision methods added to bn::rect, bn::fixed_rect, bn::top_left_rect and bn::top_left_fixed_rect.
 * * `__FILE_NAME__` macro is no longer required.
 * * Toolchain detection fixed.
 * * Build time reduced.
 *
 *
 * @section changelog_16_6_0 16.6.0
 *
 * * bn::sprites::visible and bn::sprites::set_visible added.
 * * bn::sprites actions added.
 * * Initial transparent color of the backgrounds can be specified in bn::core::init.
 * * bn::date year range not longer limited to [0..99].
 * * bn::color_effect::grayscale, bn::color_effect::blend and bn::color_effect::fade
 *   require 4-byte aligned color references with an even number of colors.
 * * Uncompressed backgrounds commit CPU usage reduced.
 * * Memory copy speed tests added to the `profiler` example.
 *
 *
 * @section changelog_16_5_0 16.5.0
 *
 * * Big maps allow wrapping (they can now be moved beyond their boundaries).
 * * Stack trace logging on error messages can be enabled with the `STACKTRACE` makefile variable.
 * * Stack trace demangle can be enabled with @ref BN_CFG_ASSERT_STACKTRACE_DEMANGLE.
 * * bn::memory::used_rom added.
 * * Import tool can now generate palettes for regular and affine background tiles.
 *   See the @ref import_regular_bg_tiles and the @ref import_affine_bg_tiles import guides
 *   to learn how to generate them.
 * * `GRAPHICS`, `AUDIO` and `DMGAUDIO` makefile variables accept file and folder paths.
 * * `bn::audio::dmg_sync_enabled` and `bn::audio::set_dmg_sync_enabled` removed
 *   as <a href="https://github.com/AntonioND/gbt-player/issues/20">they don't work properly</a>.
 * * bn::time::current 12-hour mode support.
 * * <a href="https://github.com/felixjones/agbabi">agbabi</a> updated to 2.1.6.
 * * Old documentation header restored.
 * * `dynamic_regular_bg` example shows how to deal with multiple palette ids.
 * * Tiles offset disabled in `dynamic_regular_bg` and `dynamic_affine_bg` examples.
 *
 *
 * @section changelog_16_4_0 16.4.0
 *
 * * <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a> experimental support!
 *   See the @ref getting_started_wt guide to learn how to build Butano games with it.
 * * Real time clock (RTC) support thanks to <a href="https://github.com/felixjones/agbabi">agbabi</a>.
 *   See bn::date, bn::time and the `date_time` example to learn how to deal with the RTC.
 * * bn::core::hard_reset added.
 * * bn::isprite_animate_action::current_graphics_index and bn::isprite_cached_animate_action::current_tiles added.
 * * <a href="https://github.com/felixjones/agbabi">agbabi</a> memory routines IWRAM usage reduced.
 * * Rumble management CPU usage reduced.
 * * `memcmp` optimized.
 * * Hardware access ROM usage reduced.
 * * Error messages crash fixed.
 * * Error messages with HDMA enabled have been fixed too.
 *
 *
 * @section changelog_16_3_0 16.3.0
 *
 * * bn::music::stop, bn::dmg_music::stop, bn::sound_handle::stop and bn::sound_handle::release can be called
 *   without triggering an assert if the music/sound in question isn't playing.
 * * Target `*.gba` file is rebuilt when the target `*.elf` file isn't present.
 * * Sprite affine mats management CPU usage reduced.
 * * Sound handles management CPU usage reduced.
 * * Backgrounds management CPU usage reduced.
 * * bn::unordered_map::erase and bn::unordered_set::erase fixed.
 *
 *
 * @section changelog_16_2_0 16.2.0
 *
 * * bn::dmg_music::master_volume and bn::dmg_music::set_master_volume added.
 * * bn::color_effect::blend added.
 * * Sprite affine mats management CPU usage reduced.
 * * bn::jingle::play link error fixed.
 *
 *
 * @section changelog_16_1_0 16.1.0
 *
 * * Sprite affine mats management CPU usage reduced.
 * * Music ending detection fixed.
 *
 *
 * @section changelog_16_0_0 16.0.0
 *
 * * bn::sound_handle and sound handle actions added.
 *   See the `audio` example to learn how to stop, release and setup sound effects with them.
 * * bn::jingle and jingle actions added.
 * * Audio commands generation optimized.
 * * Music ending detection fixed.
 * * Create new tiles methods deprecated.
 *
 *
 * @section changelog_15_9_0 15.9.0
 *
 * * Same maps with different tiles or palettes supported.
 * * Some clangd and Clang-Tidy warnings fixed.
 *
 *
 * @section changelog_15_8_2 15.8.2
 *
 * * bn::unordered_map::clear and bn::unordered_set::clear fixed.
 * * Backgrounds and color palettes modules documentation fixed.
 *
 *
 * @section changelog_15_8_1 15.8.1
 *
 * bn::vector::assign fixed.
 *
 *
 * @section changelog_15_8_0 15.8.0
 *
 * * Color-related H-Blank effects cleanup fixed.
 * * @ref getting_started page improved.
 * * @ref faq_memory_types question improved.
 *
 *
 * @section changelog_15_7_1 15.7.1
 *
 * Python 2 support restored.
 *
 *
 * @section changelog_15_7_0 15.7.0
 *
 * * bn::blending::restore added.
 * * bn::window::restore added.
 *
 *
 * @section changelog_15_6_0 15.6.0
 *
 * * bn::display::size added.
 * * Standard abort implemented.
 * * Color-related H-Blank effects update fixed.
 *
 *
 * @section changelog_15_5_0 15.5.0
 *
 * * bn::assert::callback and bn::assert::set_callback added.
 * * `BN_DATA_EWRAM_BSS` added.
 * * bn::memory::used_static_ewram fixed.
 * * Butano subsystems ROM usage reduced.
 * * <a href="https://www.coranac.com/man/tonclib/">Tonclib</a> tiled surface functions added.
 * * Unused <a href="https://www.coranac.com/man/tonclib/">Tonclib</a> functions and data removed.
 *
 *
 * @section changelog_15_4_1 15.4.1
 *
 * bn::core::reset race condition fixed.
 *
 *
 * @section changelog_15_4_0 15.4.0
 *
 * * bn::dmg_music now supports VGM audio files with `*.vgm` extension
 *   thanks to <a href="https://github.com/copyrat90/gbadev-ja-test">VGM player</a>.
 *   See the @ref import_dmg_music import guide and the `dmg_audio` example to learn how to import and play it.
 * * Import tool now allows to specify whether regular and affine backgrounds are big or not.
 *   See the @ref import_regular_bg and the @ref import_affine_bg import guides to learn how to specify it.
 * * bn::regular_bg_map_item::flat_layout added.
 * * @ref faq_sprites_multiple_8bpp and @ref faq_backgrounds_multiple_8bpp questions added to the @ref faq page.
 * * @ref faq_images_invalid_header_size question added to the @ref faq page.
 * * @ref import_image import guide improved.
 *
 *
 * @section changelog_15_3_2 15.3.2
 *
 * BG blocks commit fixed.
 *
 *
 * @section changelog_15_3_1 15.3.1
 *
 * Slow game pak detection fixed (again).
 *
 *
 * @section changelog_15_3_0 15.3.0
 *
 * * bn::top_left_rect and bn::top_left_fixed_rect added.
 * * bn::core::init waits for V-Blank before initializing all Butano subsystems.
 * * Slow game pak detection fixed.
 * * bn::rect issues with odd dimensions documented.
 *
 *
 * @section changelog_15_2_0 15.2.0
 *
 * * bn::seed_random added.
 * * Standard assert implemented.
 * * Thread-safe initialization of local statics disabled.
 * * Static destructors fixed.
 * * Maps commit optimized by more than 35% in some cases.
 * * Palette search optimized by more than 25% in some cases.
 * * Big maps commit optimized.
 * * Display setup commit optimized.
 * * BG blocks and sprite tiles managers ROM usage reduced.
 * * `map_collision` example added.
 *
 *
 * @section changelog_15_1_0 15.1.0
 *
 * * `memcmp` optimized.
 * * Link flags fixed.
 *
 *
 * @section changelog_15_0_0 15.0.0
 *
 * * <a href="https://sourceware.org/newlib">newlib</a> dependency removed.
 * * Standard system libraries linking can be specified in the project's `Makefile`.
 * * bn::utf8_character::size for ASCII characters fixed.
 * * More questions added to @ref faq page.
 * * @ref faq page improved.
 *
 *
 * @section changelog_14_1_0 14.1.0
 *
 * * bn::regular_bg_map_item::cells_count and bn::affine_bg_map_item::cells_count added.
 * * bn::regular_bg_map_item::decompress and bn::affine_bg_map_item::decompress fixed.
 *
 *
 * @section changelog_14_0_0 14.0.0
 *
 * * bn::regular_bg_map_item and bn::affine_bg_map_item can reference multiple maps per item.
 * * Regular and affine backgrounds import support multiple maps per image thanks to the optional `height` field.
 * * bn::regular_bg_animate_action and bn::regular_bg_cached_animate_action added.
 * * bn::affine_bg_animate_action and bn::affine_bg_cached_animate_action added.
 * * bn::pool of trivially destructible types can be destroyed without being empty.
 * * bn::list and bn::forward_list destructors fixed and optimized for trivially destructible types.
 * * Deprecated methods removed.
 *
 *
 * @section changelog_13_18_0 13.18.0
 *
 * * Sprites and sprite tiles `height` field is now optional.
 * * Standard containers ROM usage reduced.
 * * bn::unordered_map::clear and bn::unordered_set::clear optimized.
 * * bn::best_fit_allocator missing header files included.
 * * @ref BN_CFG_LOG_BACKEND documentation fixed.
 *
 *
 * @section changelog_13_17_0 13.17.0
 *
 * * bn::blending::set_transparency_alpha and bn::blending::set_intensity_alpha fixed.
 * * Tools multiprocessing pools are now closed properly.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_13_16_1 13.16.1
 *
 * bn::ibitset AND operator fixed.
 *
 *
 * @section changelog_13_16_0 13.16.0
 *
 * * bn::sound::master_volume and bn::sound::set_master_volume added.
 * * Sound master volume actions added.
 * * bn::sprite_shape_size constructor from width and height added.
 * * Maximum number of sprites per scanline increased.
 * * bn::timer::elapsed_ticks accuracy improved.
 * * Display management optimized.
 * * bn::ibitset equality operators fixed.
 * * @ref BN_LOG_BACKEND_MGBA also works with <a href="https://github.com/nba-emu/NanoBoyAdvance">NanoBoyAdvance</a>.
 * * @ref getting_started page improved.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_13_15_0 13.15.0
 *
 * * Brightness, contrast and intensity color effects optimized.
 * * bn::random internal variables are `protected` to allow child classes to access them.
 * * `compile_commands.json` generation support.
 *
 *
 * @section changelog_13_14_0 13.14.0
 *
 * * Hash support for bn::type_id_t added.
 * * Custom build base rules updated.
 * * Keypad logger startup fixed.
 * * Redundant move in bn::affine_bg_mat_attributes_hbe_ptr::create_optional removed.
 * * Parallel link-time optimization documentation updated.
 *
 *
 * @section changelog_13_13_0 13.13.0
 *
 * * bn::optional::value_type added.
 * * H-Blank effects management optimized (ROM and EWRAM usage reduced).
 * * bn::affine_bg_blending_toggle_action base class inheritance fixed.
 * * bn::regular_bg_map_cell_info size reduced.
 * * bn::optional build time reduced.
 *
 *
 * @section changelog_13_12_1 13.12.1
 *
 * <a href="https://www.coranac.com/man/tonclib/">Tonclib</a> linking error fixed.
 *
 *
 * @section changelog_13_12_0 13.12.0
 *
 * * bn::sprite_ptr::set_affine_mat optimized.
 * * bn::intrusive_forward_list::front fixed.
 * * Flickering in `world_map` example fixed.
 *
 *
 * @section changelog_13_11_0 13.11.0
 *
 * * <a href="https://github.com/felixjones/agbabi">agbabi</a> updated to 2.1.3.
 * * bn::core::current_cpu_usage, bn::core::last_cpu_usage and bn::core::last_vblank_usage accuracy improved.
 * * @ref faq page improved.
 *
 *
 * @section changelog_13_10_1 13.10.1
 *
 * `memset` undefined references with link-time optimization fixed.
 *
 *
 * @section changelog_13_10_0 13.10.0
 *
 * * `memset` moved to IWRAM.
 * * Backdrop fade blending fixed.
 *
 *
 * @section changelog_13_9_0 13.9.0
 *
 * * bn::unsafe_sqrt added.
 * * `memset` fixed.
 * * <a href="https://foopod.itch.io/sips">Sips</a> added to `README.md`.
 * * @ref faq page improved.
 *
 *
 * @section changelog_13_8_1 13.8.1
 *
 * bn::any management with types that can't be copied, moved or swapped fixed.
 *
 *
 * @section changelog_13_8_0 13.8.0
 *
 * * bn::any construction and assignment from references to non constant values fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_13_7_0 13.7.0
 *
 * * bn::igeneric_pool::contains added.
 * * bn::generic_pool::create invalid element size fixed.
 *
 *
 * @section changelog_13_6_0 13.6.0
 *
 * * bn::memory::copy and bn::memory::clear optimized.
 * * bn::array_hash fixed.
 *
 *
 * @section changelog_13_5_0 13.5.0
 *
 * * bn::music::tempo and bn::music::set_tempo added.
 * * bn::music::pitch and bn::music::set_pitch added.
 * * bn::music_tempo_to_action, bn::music_tempo_loop_action and bn::music_tempo_toggle_action added.
 * * bn::music_pitch_to_action, bn::music_pitch_loop_action and bn::music_pitch_toggle_action added.
 * * DMG music import options added.
 *   See the @ref import_dmg_music import guide to learn how to specify them.
 * * Empty bn::array allowed.
 * * bn::hash enums support.
 * * Sprites management IWRAM usage reduced.
 * * bn::array_hash optimized.
 *
 *
 * @section changelog_13_4_0 13.4.0
 *
 * * @ref BN_BASIC_ASSERT added.
 * * Repeated color palettes reduction can be disabled when importing a regular background.
 *   See the @ref import_regular_bg import guide to learn how to disable it.
 * * Unaligned byte `memset` fixed.
 * * Asserts without diagnostic information ROM usage reduced.
 * * Input tiles in `dynamic_regular_bg` and `dynamic_affine_bg` examples are imported without using a map.
 * * @ref faq page improved.
 *
 *
 * @section changelog_13_3_1 13.3.1
 *
 * bn::erase and bn::erase_if fixed.
 *
 *
 * @section changelog_13_3_0 13.3.0
 *
 * * @ref BN_CONST added.
 * * bn::sqrt, bn::atan2 and bn::degrees_atan2 marked as @ref BN_CONST.
 * * Linking with too many object files fixed.
 *
 *
 * @section changelog_13_2_0 13.2.0
 *
 * * bn::sram::read_span, bn::sram::read_span_offset, bn::sram::write_span and bn::sram::write_span_offset added.
 * * bn::core::last_missed_frames added.
 * * bn::core::set_skip_frames accuracy improved.
 * * Wait for V-Blank improved.
 * * Disabled asserts indicate the compiler that if the condition is false the code is unreachable.
 * * bn::blending_transparency_attributes missing header inclusions fixed.
 * * SRAM is cleared when formatting in the `sram` example.
 *
 *
 * @section changelog_13_1_1 13.1.1
 *
 * Recovery of deleted background maps fixed.
 *
 *
 * @section changelog_13_1_0 13.1.0
 *
 * * bn::isprite_animate_action and bn::isprite_cached_animate_action added.
 * * bn::list::remove, bn::list::remove_if, bn::forward_list::remove, bn::forward_list::remove_if,
 *   bn::intrusive_list::remove, bn::intrusive_list::remove_if, bn::intrusive_forward_list::remove and
 *   bn::intrusive_forward_list::remove_if added.
 * * Hash support for bn::istring and bn::string added.
 * * bn::best_fit_allocator (used by the heap manager) CPU usage reduced.
 * * bn::best_fit_allocator sanity check can be enabled with @ref BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED.
 * * bn::best_fit_allocator::free calls can be validated with @ref BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED.
 * * <a href="https://www.coranac.com/man/tonclib/">Tonclib</a> unused components removed
 *   (EWRAM and ROM usage reduced).
 * * bn::best_fit_allocator::realloc can shrink allocations.
 * * BPP mode can be specified when importing sprites and sprite palettes.
 *   See the @ref import_image import guide to learn how to specify it.
 * * Colors count can be specified when importing sprites, sprite tiles, sprite palettes, regular backgrounds and
 *   affine backgrounds. See the @ref import_image import guide to learn how to specify them.
 * * BMP files are validated when importing background tiles.
 * * Flipped tiles reduction disabled in `dynamic_regular_bg` example.
 * * <a href="https://forum.gbadev.net">gbadev forums</a> added to `README.md`.
 *
 *
 * @section changelog_13_0_0 13.0.0
 *
 * * <b>(Breaking change)</b> bn::best_fit_allocator (used by the heap manager) supports unlimited allocations.
 * * bn::best_fit_allocator::log_status and bn::memory::log_alloc_ewram_status added.
 * * New games made with Butano added to `README.md`.
 *
 *
 * @section changelog_12_5_0 12.5.0
 *
 * @ref BN_CFG_ASSERT_SHOW_DIAGNOSTIC added.
 *
 *
 * @section changelog_12_4_0 12.4.0
 *
 * * SRAM magic string fixed.
 * * `BN_CODE_IWRAM` macro can be used for thumb code too.
 * * Destructors of classes with `_ptr` suffix ROM usage reduced.
 * * 4BPP regular backgrounds quantization improved.
 * * Some error messages improved.
 * * @ref import_image "Import images guide" explains why <a href="https://github.com/gb-archive/usenti">Usenti</a>
 *   is recommended over other image editors.
 *
 *
 * @section changelog_12_3_0 12.3.0
 *
 * * bn::sprite_text_generator variable width fonts rendering fixed and optimized.
 * * Sprite shape and size update ROM usage reduced.
 * * Some Cppcheck warnings removed.
 *
 *
 * @section changelog_12_2_0 12.2.0
 *
 * * Automatic sprite double size detection fixed.
 * * Sprite tiles management optimized (EWRAM and CPU usage reduced).
 * * Background tiles and maps management optimized (EWRAM and CPU usage reduced).
 * * Color palettes management CPU usage reduced.
 * * Sprites and sprite affine mats management CPU usage reduced.
 * * Backgrounds management CPU usage reduced.
 * * Cameras management CPU usage reduced.
 *
 *
 * @section changelog_12_1_0 12.1.0
 *
 * * Sprite tiles delayed commit fixed.
 * * V-Blank interrupt management optimized (EWRAM and CPU usage reduced).
 * * Missed link messages frequency reduced.
 * * bn::timer::elapsed_ticks optimized.
 *
 *
 * @section changelog_12_0_0 12.0.0
 *
 * * <b>(Breaking change)</b> bn::aligned fixed.
 * * Color effects optimized.
 *
 *
 * @section changelog_11_0_1 11.0.1
 *
 * bn::string::starts_with and bn::string_view::starts_with fixed.
 *
 *
 * @section changelog_11_0_0 11.0.0
 *
 * Huffman images compression support
 * (make sure to update <a href="https://devkitpro.org/">devkitARM</a> before using it).
 *
 *
 * @section changelog_10_7_0 10.7.0
 *
 * * bn::unique_ptr move constructor, move assignment operator and bn::unique_ptr::reset
 *   allow to manage pointers and deleters with different type.
 * * bn::unique_ptr documentation improved.
 * * More questions added to @ref faq page.
 * * @ref faq page improved.
 *
 *
 * @section changelog_10_6_1 10.6.1
 *
 * bn::bitset AND, OR and XOR operators fixed.
 *
 *
 * @section changelog_10_6_0 10.6.0
 *
 * * bn::sprite_animate_action::set_wait_updates and bn::sprite_cached_animate_action::set_wait_updates added.
 * * bn::sprite_animate_action::reset and bn::sprite_cached_animate_action::reset added.
 * * Backgrounds management optimized (EWRAM and CPU usage reduced).
 * * Sprites, sprite tiles, palettes and HDMA commit CPU usage reduced.
 * * Missed link messages frequency reduced.
 * * <a href="https://mgba.io">mGBA</a> logging CPU usage reduced.
 * * Keypad logging EWRAM and maximum CPU usage reduced.
 * * CPU usage profiler entries removed.
 * * Some assert messages improved.
 *
 *
 * @section changelog_10_5_0 10.5.0
 *
 * * bn::core::vblank_callback and bn::core::set_vblank_callback added.
 * * bn::random::update added.
 * * H-Blank effects input data validation fixed.
 *
 *
 * @section changelog_10_4_0 10.4.0
 *
 * * Avoid data decompression in V-Blank if possible.
 * * Audio updates can no longer be interrupted.
 *
 *
 * @section changelog_10_3_0 10.3.0
 *
 * * bn::bg_tiles::allow_offset and bn::bg_tiles::set_allow_offset added.
 * * Import tool can now generate regular and affine background tiles without maps or palettes.
 *   See the @ref import_regular_bg_tiles and the @ref import_affine_bg_tiles import guides to learn how to import them.
 * * BPP mode can be specified when importing sprite tiles.
 *   See the @ref import_sprite_tiles import guide to learn how to specify it.
 * * Audio updates can be interrupted.
 * * SRAM reads and writes optimized.
 * * BG blocks manager ROM usage reduced.
 * * <a href="https://github.com/felixjones/agbabi">agbabi</a> memory routines updated.
 * * Missing `__FILE_NAME__` macro check added.
 * * Missing license files added.
 *
 *
 * @section changelog_10_2_0 10.2.0
 *
 * * bn::core_lock added.
 * * bn::reciprocal_16_lut added.
 * * Memory operations like `memcpy` and `memset` optimized thanks to
 *   <a href="https://github.com/felixjones/agbabi">agbabi</a>.
 * * bn::affine_mat_attributes scale setup optimized (CPU and ROM usage reduced).
 * * bn::atan2 and bn::degrees_atan2 optimized thanks to <a href="https://github.com/felixjones/agbabi">agbabi</a>.
 * * Unused interrupts removed from the interrupt manager.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_10_1_0 10.1.0
 *
 * * bn::audio::dmg_sync_enabled and bn::audio::set_dmg_sync_enabled added.
 * * Custom assembler flags can be specified in the project's `Makefile`.
 * * <a href="https://github.com/AntonioND/gbt-player">GBT Player</a> updated to 4.4.1.
 * * bn::fixed, bn::optional, bn::string_view and bn::span are no longer forward declared in the public API.
 * * Text color in assert and profile screens improved.
 * * Audio volume setup optimized.
 * * Clouds restored in `world_map` example.
 *
 *
 * @section changelog_10_0_0 10.0.0
 *
 * * Custom linker flags can be specified in the project's `Makefile`.
 * * Asserts ROM usage reduced.
 * * Audio startup fixed when link-time optimization is enabled.
 * * Audio stop fixed when link communication is active.
 * * Parallel link-time optimization temporal files ignored and added to the list of files to clean.
 *
 *
 * @section changelog_9_5_0 9.5.0
 *
 * * bn::dmg_music::left_volume, bn::dmg_music::right_volume, bn::dmg_music::set_left_volume,
 *   bn::dmg_music::set_right_volume and bn::dmg_music::set_volume added.
 * * bn::dmg_music::position and bn::dmg_music::set_position added.
 * * bn::dmg_music_volume_to_action, bn::dmg_music_volume_loop_action and bn::dmg_music_volume_toggle_action added.
 * * bn::link_state::player_count added.
 * * An external `mod2gbt` executable is no longer necessary for importing DMG music with `*.mod` extension.
 * * <a href="https://github.com/AntonioND/gbt-player">GBT Player</a> updated to 4.4.0.
 * * New interrupt manager implemented by <a href="https://github.com/AntonioND/libugba">Universal GBA Library</a>.
 * * Audio commands list management optimized (ROM, EWRAM and CPU usage reduced).
 * * Missed link messages frequency reduced.
 * * Link data sending race condition fixed.
 * * Link race condition on V-Blank fixed.
 * * Audio pause/resume <a href="https://problemkaputt.de/gba.htm">No$gba</a> error removed.
 * * Palette rotate range documentation fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_9_4_1 9.4.1
 *
 * BMP files colors count calculation fixed (again).
 *
 *
 * @section changelog_9_4_0 9.4.0
 *
 * * bn::dmg_music and bn::dmg_music_item added (yes, music playback using DMG channels is now supported
 *   thanks to <a href="https://github.com/AntonioND/gbt-player">GBT Player</a>).
 *   See the @ref import_dmg_music import guide and the `dmg_audio` example to learn how to import and play it.
 * * BMP files colors count calculation fixed.
 * * C++ keywords are not valid asset names anymore.
 * * @ref getting_started Python setup improved.
 * * <a href="https://cinemint.itch.io/tremblay-island">Tremblay Island</a> added to `README.md`.
 *
 *
 * @section changelog_9_3_0 9.3.0
 *
 * * bn::sram::clear and bn::sram::set_bytes added.
 * * bn::music::playing_item added.
 * * SRAM code moved back from ROM to EWRAM again, since it doesn't seem to crash
 *   <a href="https://problemkaputt.de/gba.htm">No$gba</a> anymore.
 * * bn::power_of_two optimized.
 * * Keypad with frame skipping fixed.
 *
 *
 * @section changelog_9_2_0 9.2.0
 *
 * * bn::regular_bg_ptr::hw_id and bn::affine_bg_ptr::hw_id added.
 * * bn::sin_and_cos, bn::degrees_sin_and_cos, bn::lut_sin_and_cos and bn::degrees_lut_sin_and_cos added.
 * * More bn::regular_bg_ptr and bn::affine_bg_ptr static contructors added.
 * * bn::regular_bg_map_item::cells_ptr and bn::affine_bg_map_item::cells_ptr added.
 * * bn::affine_mat_attributes rotation angle processing optimized.
 * * bn::unordered_map and bn::unordered_set lack of pointer stability documented.
 *
 *
 * @section changelog_9_1_1 9.1.1
 *
 * bn::calculate_sin_lut_value sine and cosine symmetry with 135 and 315 degrees fixed.
 *
 *
 * @section changelog_9_1_0 9.1.0
 *
 * * bn::system_font, bn::core::system_font and bn::core::set_system_font added.
 * * bn::core::assert_tag and bn::core::set_assert_tag added.
 * * @ref BN_VERSION_STRING added.
 * * bn::rect_window::restore_boundaries added.
 * * bn::window::set_show_all and bn::window::set_show_nothing fixed.
 * * bn::utf8_characters_map_ref::index assert shows UTF8-character data.
 * * <a href="https://github.com/laqieer/gba-free-fonts">gba-free-fonts</a> added to `README.md`.
 *
 *
 * @section changelog_9_0_0 9.0.0
 *
 * * bn::utf8_characters_map and bn::utf8_characters_map_ref added:
 *   they allow to build an UTF-8 characters map in ROM, without size limit.
 * * <b>(Breaking change)</b> bn::sprite_font now manages a bn::utf8_characters_map_ref
 *   instead of a span of UTF-8 characters.
 * * <b>(Breaking change)</b> `BN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS` removed.
 * * bn::blending_transparency_attributes::set_transparency_and_intensity_alphas added.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_8_10_0 8.10.0
 *
 * * Huffman decompression support added.
 * * bn::best_fit_allocator added.
 * * bn::istring::resize and bn::istring::shrink added.
 * * bn::string and bn::string_view compatibility improved.
 * * bn::string construction, assignment and append optimized.
 * * Integer to string conversion optimized.
 * * Slot index added to palettes manager status log.
 * * bn::calloc and bn::memory::ewram_calloc interface fixed.
 * * `.elf` files <a href="https://problemkaputt.de/gba.htm">No$gba</a> support restored.
 * * `-O0` builds support restored.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_8_9_0 8.9.0
 *
 * * Hue shift palette effect added.
 *   See the `palettes` example to learn how to use it.
 * * Brightness effect ROM usage reduced.
 * * bn::sprite_item and bn::sprite_tiles_item size reduced.
 * * Automatic sprite double size detection fixed.
 * * bn::hbe_visible_toggle_action removed, since it was not possible to use it.
 * * Clang build fixes.
 * * Clang-Tidy warnings fixed.
 * * New games made with Butano added to `README.md`.
 *
 *
 * @section changelog_8_8_0 8.8.0
 *
 * * Modified assets detection optimized.
 * * Audio and graphics tools unified in order to reduce build time.
 * * bn::newton_raphson_sqrt removed, since it doesn't work for bn::fixed input values.
 *
 *
 * @section changelog_8_7_0 8.7.0
 *
 * * An external bn::bg_palette_item can be specified when importing regular and affine backgrounds.
 * * Colors count can be specified when importing sprite and background palettes.
 * * Standard containers equality comparison optimized.
 * * Asserts file name hack fixed.
 * * <a href="https://github.com/AntonioND/libugba">Universal GBA Library</a> LGPL license files removed.
 * * Standard containers documentation improved.
 * * @ref faq page improved.
 *
 *
 * @section changelog_8_6_0 8.6.0
 *
 * * bn::random::get_int and bn::random::get_fixed added.
 * * bn::calloc, bn::realloc, bn::memory::ewram_calloc and bn::memory::ewram_realloc added.
 * * bn::unique_ptr moved to its own header file.
 * * bn::pool and bn::generic_pool ROM usage reduced.
 * * Automatic 4BPP regular backgrounds generation fixed.
 * * Asserts disabled build warnings fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_8_5_0 8.5.0
 *
 * * bn::optional::get added.
 * * bn::bitset and bn::bitset_ref optimized (managed bits count now must be a multiple of 8).
 * * ROM usage reduced due to unnecessary internal asserts removed.
 * * Blending hardware weights fixed.
 * * bn::regular_bg_map_cell_info documentation fixed.
 *
 *
 * @section changelog_8_4_0 8.4.0
 *
 * * bn::bitset_ref added.
 * * bn::diamond_angle and bn::degrees_diamond_angle added.
 * * Asserts ROM usage reduced.
 * * Unnecessary `inline` keyword usages removed.
 *
 *
 * @section changelog_8_3_0 8.3.0
 *
 * * Maps commit optimized by more than 50% in some cases.
 * * Integer alignment at minimum enforced in all containers.
 * * BG blocks manager and sprite tiles manager commit fixed.
 *
 *
 * @section changelog_8_2_0 8.2.0
 *
 * * <b>(Security issue)</b> Source code files full path is not stored in output ROMs anymore.
 * * Some Clang-Tidy and Cppcheck warnings removed.
 *
 *
 * @section changelog_8_1_0 8.1.0
 *
 * * bn::color_effect added.
 * * bn::memory::decompress added.
 * * All `uncompress` methods renamed to `decompress`.
 * * All color effects optimized.
 * * Palettes update and search optimized.
 * * @ref getting_started page improved.
 *
 *
 * @section changelog_8_0_0 8.0.0
 *
 * * Blending transparency weights can be setup separately with bn::blending::set_transparency_top_weight and
 *   bn::blending::set_transparency_bottom_weight.
 * * <b>(Breaking change)</b> bn::blending_transparency_attributes interface has changed to allow to setup
 *   transparency weights separately.
 * * Actions for blending transparency weights added.
 * * Internal graphics suffix changed from `bn_graphics` to `bn_gfx`. Remember to rebuild your project after updating.
 * * @ref faq page improved.
 *
 *
 * @section changelog_7_8_0 7.8.0
 *
 * * Generic slow game pak detection replaces SuperCard SD specific one.
 * * @ref BN_GAME_PAK_WAIT_STATE_FIRST_AUTO and @ref BN_GAME_PAK_WAIT_STATE_SECOND_AUTO enabled by default.
 * * Affine matrices update optimized.
 * * Tiles and maps managers optimized.
 * * Background windows optimized.
 * * bn::audio::update_on_vblank and bn::audio::set_update_on_vblank documentation improved.
 * * @ref faq page improved.
 *
 *
 * @section changelog_7_7_0 7.7.0
 *
 * * bn::audio::update_on_vblank and bn::audio::set_update_on_vblank added.
 * * Sprite tiles update and commit optimized.
 * * Background tiles and maps update and commit optimized.
 * * Big maps commit optimized.
 * * Asserts ROM usage reduced.
 * * H-Blank effects ROM usage reduced.
 * * bn::sprite_text_generator ROM usage reduced.
 *
 *
 * @section changelog_7_6_0 7.6.0
 *
 * * Display and audio hardware commit optimized.
 * * bn::unique_ptr size reduced.
 * * bn::regular_bg_ptr::set_item and bn::affine_bg_ptr::set_item fixed.
 * * @ref faq page improved.
 * * @ref main_licenses section improved.
 *
 *
 * @section changelog_7_5_1 7.5.1
 *
 * Audio and HDMA issues when updating at less than 60FPS fixed.
 *
 *
 * @section changelog_7_5_0 7.5.0
 *
 * * bn::sprites::reserved_handles_count and bn::sprites::set_reserved_handles_count added.
 * * @ref BN_CFG_PROFILER_LOG_ENGINE_DETAILED added.
 * * Sprites and backgrounds decompression optimized thanks to
 *   <a href="https://github.com/Cult-of-GBA/BIOS">Cult-of-GBA BIOS</a>.
 * * Sprites and backgrounds hardware setup optimized.
 * * Sprites automatic double size mode optimized.
 * * Affine sprites hiding fixed.
 *
 *
 * @section changelog_7_4_0 7.4.0
 *
 * * Run-length is preferred to LZ77 when choosing best compression mode.
 * * Sprites automatic double size mode and affine matrix management fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_7_3_0 7.3.0
 *
 * * bn::sprite_text_generator can now plot multiple 16x16 variable width characters in a single sprite.
 * * bn::sprite_text_generator IWRAM usage reduced.
 * * Third party libraries to link can be specified in the project's `Makefile`.
 * * Project's `Makefile` allows to build source files generated with an external tool.
 *   See the `external_tool` example to learn how to do it.
 * * `butano-audio-tool.py` generates a list of all available music and sound items with their name.
 * * `audio_player` example added.
 * * <a href="https://gvaliente.itch.io/varooom-3d">Varooom 3D</a> improved
 * (check its `readme.txt` file for details).
 *
 *
 * @section changelog_7_2_0 7.2.0
 *
 * * bn::regular_bg_map_cell_info and bn::affine_bg_map_cell_info added.
 * * bn::regular_bg_map_item::cell_index and bn::regular_bg_map_item::cell added.
 * * bn::affine_bg_map_item::cell_index and bn::affine_bg_map_item::cell added.
 * * Maps commit optimized by more than 200% in some cases.
 * * `dynamic_regular_bg` example added.
 * * `dynamic_bg` example renamed to `dynamic_affine_bg` and improved.
 *
 *
 * @section changelog_7_1_1 7.1.1
 *
 * bn::vector, bn::unordered_map and bn::unordered_set move constructors and assignment operators fixed.
 *
 *
 * @section changelog_7_1_0 7.1.0
 *
 * * HDMA commit fixed.
 * * @ref faq page improved.
 * * Games made with Butano added to `README.md`.
 *
 *
 * @section changelog_7_0_1 7.0.1
 *
 * bn::format build fix.
 *
 *
 * @section changelog_7_0_0 7.0.0
 *
 * * <a href="https://gvaliente.itch.io/varooom-3d">Varooom 3D</a> source code and assets added.
 * * Maximum number of active H-Blank effects reduced to 6.
 * * Common files moved to `common` namespace.
 *
 *
 * @section changelog_6_18_0 6.18.0
 *
 * * Generated items are now `constexpr inline` to avoid being copied to every translation unit.
 * * Sprites EWRAM usage reduced.
 * * Different type backgrounds sorting fixed.
 *
 *
 * @section changelog_6_17_1 6.17.1
 *
 * * bn::sort with 128bit sort keys fixed.
 * * IRQs setup during startup improved.
 *
 *
 * @section changelog_6_17_0 6.17.0
 *
 * * bn::memory::used_stack_iwram added.
 * * bn::sort optimized.
 * * VRAM allocation error messages improved.
 * * bn::optional::emplace copy avoided.
 * * @ref import guide improved.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_6_16_0 6.16.0
 *
 * * bn::sin_lut is not copied to every translation unit.
 * * bn::reciprocal_lut is not copied to every translation unit.
 * * GBA keypad is read after playing all keypad commands.
 * * bn::degrees_lut_cos assert added.
 *
 *
 * @section changelog_6_15_0 6.15.0
 *
 * * bn::rect::top_right, bn::rect::bottom_left, bn::fixed_rect::top_right and bn::fixed_rect::bottom_left added.
 * * GPIO register is not setup unless needed (avoids <a href="https://mgba.io">mGBA</a> warning at startup).
 * * SRAM code moved from EWRAM to ROM in order to avoid a
 *   <a href="https://problemkaputt.de/gba.htm">No$gba</a> error.
 * * bn::abs for unsigned arguments improved.
 * * `*.elfbin` output files renamed to `*.elf` as it should.
 * * bn::unordered_map::erase and bn::unordered_set::erase fixed.
 *
 *
 * @section changelog_6_14_0 6.14.0
 *
 * * Asserts ROM usage reduced.
 * * bn::deque::push_front, bn::deque::emplace_front and bn::deque::insert fixed.
 *
 *
 * @section changelog_6_13_1 6.13.1
 *
 * * bn::to_array fixed.
 * * bn::sprite_text_generator documentation fixed.
 *
 *
 * @section changelog_6_13_0 6.13.0
 *
 * * EWRAM wait states count can be specified with @ref BN_CFG_EWRAM_WAIT_STATE.
 * * bn::fixed_t::floor_integer and bn::fixed_t::ceil_integer added.
 * * bn::keypad::any_held, bn::keypad::any_pressed and bn::keypad::any_released added.
 * * Unnecessary GPIO register writes avoided.
 * * bn::type_id marked as `constexpr`.
 * * Background move actions fixed.
 * * `constexpr const` replaced with `constexpr` where it makes sense.
 *
 *
 * @section changelog_6_12_0 6.12.0
 *
 * * Multiplication and division methods added to bn::fixed_point, bn::fixed_size and bn::fixed_rect.
 * * Colors count added to palettes manager status log.
 * * Invalid bn::fixed_t unsigned operator overloads removed.
 *
 *
 * @section changelog_6_11_0 6.11.0
 *
 * * An external tool can be called from the project's `Makefile`.
 *   See the `external_tool` example to learn how to call it.
 * * Affine background tiles offset optimized.
 * * Big numbers to string conversion optimized.
 * * 32bit integer division optimized.
 * * 64bit integer division by 0 assert added.
 * * Big maps maximum valid position fixed.
 * * Compressed tiles warning fixed.
 * * GCC11 warning fixed.
 * * Some 8x8 sprite text characters fixed.
 * * @ref qt_creator added.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_6_10_0 6.10.0
 *
 * * `dynamic_bg` example added.
 * * Affine background tiles offset fixed.
 *
 *
 * @section changelog_6_9_0 6.9.0
 *
 * * bn::sprite_font supports all sprite shapes and sizes.
 * * Trivial type containers destruction optimized.
 * * Container destructors are always `noexcept`.
 * * Redundant container asserts removed.
 * * GPIO direction register setup.
 *
 *
 * @section changelog_6_8_0 6.8.0
 *
 * * Sprites optimized.
 * * Blending optimized.
 * * Mosaic optimized.
 * * Internal memory transfers optimized.
 * * `texture_polygons` example affine parameters fixed.
 * * Clouds removed from `world_map` example.
 *
 *
 * @section changelog_6_7_0 6.7.0
 *
 * * Automatic sprite double size detection improved.
 * * Butano Fighter allows to disable rumble in the title menu.
 *
 *
 * @section changelog_6_6_2 6.6.2
 *
 * Automatic sprite double size detection fixed.
 *
 *
 * @section changelog_6_6_1 6.6.1
 *
 * BG blocks manager use after move fixed.
 *
 *
 * @section changelog_6_6_0 6.6.0
 *
 * * SRAM memory usage reduced.
 * * Sprite animate actions memory usage reduced.
 * * Missing asserts added to some actions.
 * * bn::random is now a trivial class.
 * * BG blocks manager search fixed.
 * * `world_map` example frame rate back to 60FPS.
 *
 *
 * @section changelog_6_5_0 6.5.0
 *
 * * bn::bitset and bn::ibitset added.
 * * BG blocks manager and sprite tiles manager allocation fixed.
 *
 *
 * @section changelog_6_4_0 6.4.0
 *
 * * Sprites and affine backgrounds support horizontal and vertical shear.
 * * bn::affine_mat_attributes::unsafe_set_register_values added.
 * * `texture_polygons` example added.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_6_3_0 6.3.0
 *
 * * @ref BN_VERSION_MAJOR, @ref BN_VERSION_MINOR and @ref BN_VERSION_PATCH added.
 * * bn::core::current_cpu_usage, bn::core::last_cpu_usage and bn::core::last_vblank_usage accuracy improved.
 * * Signed division fixed and optimized.
 * * BG blocks manager and sprite tiles manager allocation fixed.
 * * BG blocks manager prints requested blocks count when an allocation fails.
 *
 *
 * @section changelog_6_2_0 6.2.0
 *
 * * bn::core::skip_frames and bn::core::set_skip_frames added.
 * * bn::sprite_font usability improved.
 * * bn::reciprocal_lut and bn::lut_reciprocal fixed.
 *
 *
 * @section changelog_6_1_0 6.1.0
 *
 * * Sprite tiles generation outputs sprite shape and size.
 * * @ref BN_ASSERT and @ref BN_ERROR can be modified or disabled for specific code sections.
 * * Sprites IWRAM usage reduced.
 * * Some internal asserts removed.
 * * Profiler screen fixed.
 * * Documentation explains how to place data in ROM.
 *
 *
 * @section changelog_6_0_0 6.0.0
 *
 * * Rumble support (if your cart supports it). See @ref rumble and the `rumble` example.
 * * bn::sprites::reload added.
 * * bn::hdma::high_priority_start and bn::hdma::high_priority_stop added.
 * * bn::hdma::running and bn::hdma::high_priority_running added.
 * * Sprite tiles can be generated from images alone, without palettes.
 * * Graphics and audio subfolders supported.
 * * bn::core::reset fixed.
 * * HDMA disable when an assert or the profiler are shown fixed.
 * * HDMA first screen horizontal line fixed.
 * * bn::newton_raphson_sqrt recursion avoided.
 * * Python install guide improved.
 *
 *
 * @section changelog_5_3_1 5.3.1
 *
 * bn::atan2 and bn::degrees_atan2 fixed.
 *
 *
 * @section changelog_5_3_0 5.3.0
 *
 * * bn::sin, bn::cos, bn::atan2 and bn::degrees_atan2 added.
 * * H-Blank effects optimized (thanks JoaoBaptMG).
 * * bn::calculate_sin_lut_value optimized.
 * * bn::lut_reciprocal optimized.
 * * @ref faq page improved.
 *
 *
 * @section changelog_5_2_0 5.2.0
 *
 * * bn::sprite_ptr::put_below, bn::regular_bg_ptr::put_below and bn::affine_bg_ptr::put_below added.
 * * H-Blank effects IWRAM usage reduced.
 * * `butano-graphics-tool.py` errors reporting improved.
 *
 *
 * @section changelog_5_1_0 5.1.0
 *
 * * Automatic game pak wait states setup (see @ref BN_GAME_PAK_WAIT_STATE_FIRST_AUTO and
 *   @ref BN_GAME_PAK_WAIT_STATE_SECOND_AUTO for more information).
 * * bn::format and bn::format_ref functions added.
 * * Some methods to indicate if a background or a background map are big or not have been added.
 * * bn::calculate_sin_lut_value precision improved.
 * * @ref faq page added.
 * * `butano-graphics-tool.py` now uses all CPU cores.
 * * Missing third party license files included.
 *
 *
 * @section changelog_5_0_0 5.0.0
 *
 * * Compressed sprites and backgrounds support (see @ref import to learn how to generate them).
 * * bn::degrees_sin and bn::degrees_cos don't use a LUT anymore,
 *   they are replaced by bn::degrees_lut_sin and bn::degrees_lut_cos.
 * * bn::lut_sin and bn::lut_cos input angle range increased to [0..2048].
 * * Enum classes size reduced.
 * * Required fields when importing assets are not defaulted anymore.
 * * bn::rule_of_three_approximation overflow fixed.
 *
 *
 * @section changelog_4_4_0 4.4.0
 *
 * * bn::sprite_font allows to specify space between characters.
 * * Background palettes bits per pixel mode can be specified by the user.
 * * Palettes change optimized.
 * * Unused palette colors are not imported anymore.
 * * bn::sprite_ptr::set_tiles() validation fixed.
 *
 *
 * @section changelog_4_3_0 4.3.0
 *
 * * H-Blank effects EWRAM usage reduced (more than 2KB by default).
 * * Optimization level changed to `-O2` to avoid another
 *   <a href="https://problemkaputt.de/gba.htm">No$gba</a> crash.
 * * SRAM code moved back from ROM to EWRAM, since doing that doesn't make
 *   <a href="https://problemkaputt.de/gba.htm">No$gba</a> crash anymore.
 * * Redundant reset code removed.
 *
 *
 * @section changelog_4_2_0 4.2.0
 *
 * * Sprite and background palettes can be generated from images alone, without tiles or maps.
 * * `<cstddef>` is always included (`bn_cstddef.h` header file removed).
 * * bn::unordered_map and bn::unordered_set assignment fixed.
 *
 *
 * @section changelog_4_1_0 4.1.0
 *
 * * H-Blank effects optimized (it fixes `world_map` example flickering).
 * * SRAM code moved from EWRAM to ROM to avoid a <a href="https://problemkaputt.de/gba.htm">No$gba</a> crash.
 *
 *
 * @section changelog_4_0_0 4.0.0
 *
 * * Affine backgrounds support.
 *   See @ref affine_bg and the `affine_bgs`, `mode_7` and `world_map` examples for more.
 * * bn::hblank_effect_ptr renamed to bn::hbe_ptr (all child classes have been renamed too).
 * * bn::sprite_affine_mat_attributes renamed to bn::affine_mat_attributes.
 * * bn::bg_tiles_ptr renamed to bn::regular_bg_tiles_ptr.
 * * bn::bg_tiles_item renamed to bn::regular_bg_tiles_item.
 * * bn::affine_mat_scale_lut replaced with bn::reciprocal_lut.
 * * bn::core::cpu_usage() and bn::core::vblank_usage() renamed to
 *   bn::core::last_cpu_usage() and bn::core::last_vblank_usage().
 * * bn::lut_reciprocal() added.
 * * bn::core::current_cpu_usage() added.
 * * <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a> updated to v2.1.4.
 * * HDMA is disabled when needed, like before sleeping or before an assert screen.
 * * bn::regular_bg_tiles_item::valid_tiles_count() fixed.
 * * bn::nullopt documentation links fixed.
 *
 *
 * @section changelog_3_3_0 3.3.0
 *
 * * HDMA properly supported (now it works at less than 60fps). See bn::hdma and the `hdma_polygons` example for more.
 * * <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a> remote timeout detection fixed.
 *
 *
 * @section changelog_3_2_1 3.2.1
 *
 * bn::optional build fix.
 *
 *
 * @section changelog_3_2_0 3.2.0
 *
 * bn::optional is now `constexpr`.
 *
 *
 * @section changelog_3_1_0 3.1.0
 *
 * * bn::regular_bg_map_ptr::tiles_offset() and bn::regular_bg_map_ptr::palette_banks_offset() methods added.
 * * Allocated background tiles and maps commit fixed.
 *
 *
 * @section changelog_3_0_0 3.0.0
 *
 * Thanks to the awesome <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a>,
 * multiplayer support has been implemented! See bn::link and the `link` example for more.
 *
 *
 * @section changelog_2_0_0 2.0.0
 *
 * * By removing some method overloads, lots of runtime asserts when creating resources have been removed.
 * * bn::palette_bpp_mode has been renamed to bn::bpp_mode and `bpp_mode()` methods have been renamed to `bpp()`.
 * * 8 bits per pixel background tiles allocation fixed.
 *
 *
 * @section changelog_1_0_0 1.0.0
 *
 * * Regular maps bigger than 512x512 supported.
 * * 32bit integer division by 0 assert added.
 * * bn::regular_bg_item maximum tiles check added.
 * * bn::regular_bg_item generation options fixed.
 *
 *
 * @section changelog_0_4_0 0.4.0
 *
 * * `btn` renamed to `bn`. No more API breaks will be made between minor releases after 1.0.0, promise.
 * * Background tiles manager status can be printed in the log with bn::bg_tiles::log_status().
 *   This is done automatically when a non-optional background tiles allocation fails too.
 * * Background regular maps manager status can be printed in the log with bn::bg_maps::log_status().
 *   This is done automatically when a non-optional regular background map allocation fails too.
 * * Sprite tiles manager status can be printed in the log with bn::sprite_tiles::log_status().
 *   This is done automatically when a non-optional sprite tiles allocation fails too.
 * * Color palettes managers status can be printed in the log with
 *   bn::bg_palettes::log_status() and bn::sprite_palettes::log_status().
 *   This is done automatically when a non-optional color palette allocation fails too.
 * * Sprites destruction optimized.
 * * Setters with an optional parameter added to some classes.
 * * Optional components documentation fixed.
 * * Other documentation improvements.
 *
 *
 * @section changelog_0_3_0 0.3.0
 *
 * * Sprites update performance improved up to 30% in Butano Fighter
 *   thanks to avoid rebuilding sprites list as much as possible.
 * * Profiler can show the maximum measured ticks per entry.
 * * Assets tools print output binaries size.
 *
 *
 * @section changelog_0_2_0 0.2.0
 *
 * * Performance improved up to 12% in Butano Fighter without `-flto` thanks to using less build translation units.
 * * Documentation improved.
 *
 *
 * @section changelog_0_1_0 0.1.0
 *
 * First release.
 */

#endif
