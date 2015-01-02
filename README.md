tween
=====

- Tween is a lightweight easing library. Written in C++03.
- Tween provides optional memoization and chaining.
- Tween is self-contained. Only math.h header is required.
- Tween is cross-platform.
- Tween is header-only.
- Tween is tiny.
- Tween is BOOST licensed.

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
BOUNCEIN_01
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
QUADIN_01* BOUNCEIN_01
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
    LINEAR_01,

    QUADIN_01,          // t^2
    QUADOUT_01,
    QUADINOUT_01,
    CUBICIN_01,         // t^3
    CUBICOUT_01,
    CUBICINOUT_01,
    QUARTIN_01,         // t^4
    QUARTOUT_01,
    QUARTINOUT_01,
    QUINTIN_01,         // t^5
    QUINTOUT_01,
    QUINTINOUT_01,
    SINEIN_01,          // sin(t)
    SINEOUT_01,
    SINEINOUT_01,
    EXPOIN_01,          // 2^t
    EXPOOUT_01,
    EXPOINOUT_01,
    CIRCIN_01,          // sqrt(1-t^2)
    CIRCOUT_01,
    CIRCINOUT_01,
    ELASTICIN_01,       // exponentially decaying sine wave
    ELASTICOUT_01,
    ELASTICINOUT_01,
    BACKIN_01,          // overshooting cubic easing: (s+1)*t^3 - s*t^2
    BACKOUT_01,
    BACKINOUT_01,
    BOUNCEIN_01,        // exponentially decaying parabolic bounce
    BOUNCEOUT_01,
    BOUNCEINOUT_01,

    SINESQUARE_01,      // gapjumper's
    EXPONENTIAL_01,     // gapjumper's
    SCHUBRING1_01,      // terry schubring's formula 1
    SCHUBRING2_01,      // terry schubring's formula 2
    SCHUBRING3_01,      // terry schubring's formula 3

    SINPI2_01,          // tomas cepeda's
    SWING_01,           // tomas cepeda's & lquery's

    TOTAL,
    UNDEFINED
};
```

## refs

- [juice it or lose it](https://www.youtube.com/watch?v=Fy0aCDmgnxg)
- [easings](http://gizma.com/easing)

