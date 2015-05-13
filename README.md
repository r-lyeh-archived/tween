tween <a href="https://travis-ci.org/r-lyeh/tween"><img src="https://api.travis-ci.org/r-lyeh/tween.svg?branch=master" align="right" /></a>
=====

- Tween is a lightweight easing library. Written in C++03.
- Tween provides optional memoization and chaining.
- Tween is self-contained. Only math.h header is required.
- Tween is cross-platform.
- Tween is header-only.
- Tween is tiny.
- Tween is zlib/libpng licensed.

## sample
```c++
double value, t01 = 0.5;
value = tween::bouncein( t01 ); // -> 0.234375
value = tween::bouncein( tween::quadin( t01) ); // -> 0.0273438
value = tween::bouncein( t01 ) * tween::quadin( t01 ); // -> 0.0585937
```

## possible output
```
$tween: ./sample.out 28
BOUNCEIN
------------------------------------------------------------------------+$#####*     1
                                                                     +*$$******$     0
                                                                   +*$***$$$$$*+     0
                                                                 +*$**$$*+           0
                                                                +***$*+              0
                                                              +*$*$*+                0
                                                             +****+                  0
                                                            +**$*+                   0
                                                           +***+                     0
                                                          +***+                      0
                                                         +***+                       0
                                                        +***+                        0
                                                       +***+                         0
                                                      +***+                          0
                              +*$$$$$$$$$*+          +***+                           0
                           +*$$***********$$*+      +***+                            0
                        +*$$***$$$$$$$$$$$***$*+   +***+                             0
           +*$$$$*+    +****$$*+         +*$$**$*+ ++*+                              0
$$$$$$$$$$$$******$$$$$$*$$*+               +*$**$$$*+                               0
*$$$$$$$$$$$######$$$$$$$+--------------------+$#$$++--------------------------- (v) 0
0.000000                                                                0.000000 (t)
Min: 0; Max: 1; Avg: 0; Hit: 0; Lapse: 0s; Avg Hits/sec: 0; Cur: 0

$tween: ./sample.out -1 28
QUADIN* BOUNCEIN
--------------------------------------------------------------------------------     1
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                                                     0
                                                         +*$$$$$$$$$*+               0
                            +*$$$$$$$$$$$*+            +*$***********$$$*+           0
                         +*$$*************$$*+       +*$**$$$$$$$$$$$****$*+         0
           +*$$$*+     +*$***$$$$$$$$$$$$$***$$*+  +*$**$*+         +*$$$**$*+       0
$$$$$$$$$$$$*****$$$$$$$**$$*+           +*$$***$$$$**$*+               +*$**$$$     0
*$$$$$$$$$$$#####$$$$$$$#$+-----------------+$##$$$$#$+-------------------+$#$$* (v) 0
0.000000                                                                0.000000 (t)
Min: 0; Max: 1; Avg: 0; Hit: 0; Lapse: 0s; Avg Hits/sec: 0; Cur: 0
```

## API
```c++
// basics f(t) = t ; memoized
double linear( double dt01 );

// penner's f(t) = t^2 ; memoized
double quadin( double dt01 );
double quadout( double dt01 );
double quadinout( double dt01 );

// penner's f(t) = t^3 ; memoized
double cubicin( double dt01 );
double cubicout( double dt01 );
double cubicinout( double dt01 );

// penner's f(t) = t^4 ; memoized
double quartin( double dt01 );
double quartout( double dt01 );
double quartinout( double dt01 );

// penner's f(t) = t^5 ; memoized
double quintin( double dt01 );
double quintout( double dt01 );
double quintinout( double dt01 );

// penner's f(t) = sin(t) ; memoized
double sinein( double dt01 );
double sineout( double dt01 );
double sineinout( double dt01 );

// penner's f(t) = 2^t ; memoized
double expoin( double dt01 );
double expoout( double dt01 );
double expoinout( double dt01 );

// penner's f(t) = sqrt(1-t^2) ; memoized
double circin( double dt01 );
double circout( double dt01 );
double circinout( double dt01 );

// penner's exponentially decaying sine wave ; memoized
double elasticin( double dt01 );
double elasticout( double dt01 );
double elasticinout( double dt01 );

// penner's overshooting cubic easing f(t) = (s+1)*t^3 - s*t^2 ; memoized
double backin( double dt01 );
double backout( double dt01 );
double backinout( double dt01 );

// penner's exponentially decaying parabolic bounce ; memoized
double bouncein( double dt01 );
double bounceout( double dt01 );
double bounceinout( double dt01 );

// gapjumper's ; memoized
double sinesquare( double dt01 );
double exponential( double dt01 );

// terry schubring's ; memoized
double terrys1( double dt01 );
double terrys2( double dt01 );
double terrys3( double dt01 );

// lQuery's and tomas cepeda's ; memoized
double swing( double dt01 );
double sinpi2( double dt01 );

// modifiers
double ping( double dt01 );
double pong( double dt01 );
double pingpong( double dt01 );
double pongping( double dt01 );

template<typename EASE> double in( double dt01, EASE &ease );
template<typename EASE> double out( double dt01, EASE &ease );
template<typename EASE> double inout( double dt01, EASE &ease );

// generics
double ease( int type, double dt01, bool memoized = false );
const char *nameof( int type );

enum TYPE
{
    LINEAR,

    QUADIN,          // t^2
    QUADOUT,
    QUADINOUT,
    CUBICIN,         // t^3
    CUBICOUT,
    CUBICINOUT,
    QUARTIN,         // t^4
    QUARTOUT,
    QUARTINOUT,
    QUINTIN,         // t^5
    QUINTOUT,
    QUINTINOUT,
    SINEIN,          // sin(t)
    SINEOUT,
    SINEINOUT,
    EXPOIN,          // 2^t
    EXPOOUT,
    EXPOINOUT,
    CIRCIN,          // sqrt(1-t^2)
    CIRCOUT,
    CIRCINOUT,
    ELASTICIN,       // exponentially decaying sine wave
    ELASTICOUT,
    ELASTICINOUT,
    BACKIN,          // overshooting cubic easing: (s+1)*t^3 - s*t^2
    BACKOUT,
    BACKINOUT,
    BOUNCEIN,        // exponentially decaying parabolic bounce
    BOUNCEOUT,
    BOUNCEINOUT,

    SINESQUARE,      // gapjumper's
    EXPONENTIAL,     // gapjumper's
    SCHUBRING1,      // terry schubring's formula 1
    SCHUBRING2,      // terry schubring's formula 2
    SCHUBRING3,      // terry schubring's formula 3

    SINPI2,          // tomas cepeda's
    SWING,           // tomas cepeda's & lquery's

    TOTAL,
    UNDEFINED
};
```

## refs

- [juice it or lose it](https://www.youtube.com/watch?v=Fy0aCDmgnxg)
- [easings](http://gizma.com/easing)
- [easings](http://easings.net/)
