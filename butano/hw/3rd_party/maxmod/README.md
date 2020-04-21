# maxmod

This is a [maxmod](https://maxmod.devkitpro.org/) fork with two small but useful additions.

## Module playback

`mmGetPosition()` allows to retrieve active modules's current position.

Since it can be confusing than `mmPosition()` is a setter and not a getter, an alias called `mmSetPosition()` has been added too:

```cpp
/****************************************************************************
 * mmGetPosition()
 *
 * Get playback position.
 ****************************************************************************/

mm_word mmGetPosition( void );

/****************************************************************************
 * mmSetPosition()
 *
 * Set playback position (alias for mmPosition()).
 *
 * position: New position in the module sequence.
 ****************************************************************************/

inline void mmSetPosition( mm_word position )
{
    mmPosition( position );
}
```

## Sound effects

`mmEffectActive()` allows to determine if a sound effect is active or not:

```cpp
/****************************************************************************
 * mmEffectActive( mm_sfxhand handle )
 *
 * Indicates if a sound effect is active or not.
 *
 * handle : Sound effect handle.
 ****************************************************************************/

mm_bool mmEffectActive( mm_sfxhand handle );
```
