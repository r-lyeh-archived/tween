/* Tween, a lightweight easing library. zlib/libpng licensed.
 * - rlyeh.

 * Based on code by Robert Penner, GapJumper, Terry Schubring, Jesus Gollonet,
 * Tomas Cepeda, John Resig, lQuery team, Warren Moore. Thanks guys! :-)
 */

#pragma once
#include <cmath>

namespace tween
{
    // public API 

    // basics f(t) = t ; memoized
    // double linear( double dt01 );

    // penner's f(t) = t^2 ; memoized
    // double quadin( double dt01 );
    // double quadout( double dt01 );
    // double quadinout( double dt01 );

    // penner's f(t) = t^3 ; memoized
    // double cubicin( double dt01 );
    // double cubicout( double dt01 );
    // double cubicinout( double dt01 );

    // penner's f(t) = t^4 ; memoized
    // double quartin( double dt01 );
    // double quartout( double dt01 );
    // double quartinout( double dt01 );

    // penner's f(t) = t^5 ; memoized
    // double quintin( double dt01 );
    // double quintout( double dt01 );
    // double quintinout( double dt01 );

    // penner's f(t) = sin(t) ; memoized
    // double sinein( double dt01 );
    // double sineout( double dt01 );
    // double sineinout( double dt01 );

    // penner's f(t) = 2^t ; memoized
    // double expoin( double dt01 );
    // double expoout( double dt01 );
    // double expoinout( double dt01 );

    // penner's f(t) = sqrt(1-t^2) ; memoized
    // double circin( double dt01 );
    // double circout( double dt01 );
    // double circinout( double dt01 );

    // penner's exponentially decaying sine wave ; memoized
    // double elasticin( double dt01 );
    // double elasticout( double dt01 );
    // double elasticinout( double dt01 );

    // penner's overshooting cubic easing f(t) = (s+1)*t^3 - s*t^2 ; memoized
    // double backin( double dt01 );
    // double backout( double dt01 );
    // double backinout( double dt01 );

    // penner's exponentially decaying parabolic bounce ; memoized
    // double bouncein( double dt01 );
    // double bounceout( double dt01 );
    // double bounceinout( double dt01 );

    // gapjumper's ; memoized
    // double sinesquare( double dt01 );
    // double exponential( double dt01 );

    // terry schubring's ; memoized
    // double terrys1( double dt01 );
    // double terrys2( double dt01 );
    // double terrys3( double dt01 );

    // lQuery's and tomas cepeda's ; memoized
    // double swing( double dt01 );
    // double sinpi2( double dt01 );

    // modifiers
    // double ping( double dt01 );
    // double pong( double dt01 );
    // double pingpong( double dt01 );
    // double pongping( double dt01 );

    // template<typename EASE> double in( double dt01, EASE &ease );
    // template<typename EASE> double out( double dt01, EASE &ease );
    // template<typename EASE> double inout( double dt01, EASE &ease );

    // generics
    // double ease( int type, double dt01, bool memoized = false );
    // const char *nameof( int type );

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

    // }

    // implementation

    static inline //constexpr
    double ease( int easetype, double t, bool memoized = false )
    {
        enum { LUT_SLOTS = 256 }; 
        static float lut[TOTAL][LUT_SLOTS], *init = 0; 
        if( !init ) {
            init = &lut[0][0];
            for( int j = 0; j < TOTAL; ++j ) {
                for( int i = 0; i < LUT_SLOTS; ++i ) {
                    lut[j][i] = ease( j, double(i) / (LUT_SLOTS-1), false );
                }
            }
        }

        using namespace std;

        const double is_looped = 0;     /* used to be a param long time ago */
        const double d = 1.f;           /* used to be a param long time ago */ /* (d)estination, final time */
        const double pi = 3.1415926535897932384626433832795;
        const double pi2 = 3.1415926535897932384626433832795 / 2;

        /* tiny optimizations { */

        if( is_looped ) {
            if( t < 0 ) {
                t = -t;
            }
            // todo: optimize me?
            while( t > d ) {
                t -= d;
            }            
        } else {
            // clamp
            if( t < 0.f ) {
                t = 0;
            } 
            else
            if( t > d ) {
                t = d;
            }
        }

        if( memoized ) {
            return lut[ easetype ][ int(t*(LUT_SLOTS-1)) ];
        }

        /* } */

        double p = t/d;

        switch( easetype )
        {
            // Modeled after the line y = x
            default:
            case TYPE::LINEAR: {
                return p;
            }

            // Modeled after the parabola y = x^2
            case TYPE::QUADIN: {
                return p * p;
            }

            // Modeled after the parabola y = -x^2 + 2x
            case TYPE::QUADOUT: {
                return -(p * (p - 2));
            }

            // Modeled after the piecewise quadratic
            // y = (1/2)((2x)^2)             ; [0, 0.5)
            // y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
            case TYPE::QUADINOUT: {
                if(p < 0.5) {
                    return 2 * p * p;
                }
                else {
                    return (-2 * p * p) + (4 * p) - 1;
                }
            }

            // Modeled after the cubic y = x^3
            case TYPE::CUBICIN: {
                return p * p * p;
            }

            // Modeled after the cubic y = (x - 1)^3 + 1
            case TYPE::CUBICOUT: {
                double f = (p - 1);
                return f * f * f + 1;
            }

            // Modeled after the piecewise cubic
            // y = (1/2)((2x)^3)       ; [0, 0.5)
            // y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
            case TYPE::CUBICINOUT: {
                if(p < 0.5) {
                    return 4 * p * p * p;
                }
                else {
                    double f = ((2 * p) - 2);
                    return 0.5 * f * f * f + 1;
                }
            }

            // Modeled after the quartic x^4
            case TYPE::QUARTIN: {
                return p * p * p * p;
            }

            // Modeled after the quartic y = 1 - (x - 1)^4
            case TYPE::QUARTOUT: {
                double f = (p - 1);
                return f * f * f * (1 - p) + 1;
            }

            // Modeled after the piecewise quartic
            // y = (1/2)((2x)^4)        ; [0, 0.5)
            // y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
            case TYPE::QUARTINOUT:  {
                if(p < 0.5) {
                    return 8 * p * p * p * p;
                }
                else {
                    double f = (p - 1);
                    return -8 * f * f * f * f + 1;
                }
            }

            // Modeled after the quintic y = x^5
            case TYPE::QUINTIN:  {
                return p * p * p * p * p;
            }

            // Modeled after the quintic y = (x - 1)^5 + 1
            case TYPE::QUINTOUT:  {
                double f = (p - 1);
                return f * f * f * f * f + 1;
            }

            // Modeled after the piecewise quintic
            // y = (1/2)((2x)^5)       ; [0, 0.5)
            // y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
            case TYPE::QUINTINOUT:  {
                if(p < 0.5) {
                    return 16 * p * p * p * p * p;
                }
                else {
                    double f = ((2 * p) - 2);
                    return  0.5 * f * f * f * f * f + 1;
                }
            }

            // Modeled after quarter-cycle of sine wave
            case TYPE::SINEIN: {
                return sin((p - 1) * pi2) + 1;
            }

            // Modeled after quarter-cycle of sine wave (different phase)
            case TYPE::SINEOUT: {
                return sin(p * pi2);
            }

            // Modeled after half sine wave
            case TYPE::SINEINOUT: {
                return 0.5 * (1 - cos(p * pi));
            }

            // Modeled after shifted quadrant IV of unit circle
            case TYPE::CIRCIN: {
                return 1 - sqrt(1 - (p * p));
            }

            // Modeled after shifted quadrant II of unit circle
            case TYPE::CIRCOUT: {
                return sqrt((2 - p) * p);
            }

            // Modeled after the piecewise circular function
            // y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
            // y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
            case TYPE::CIRCINOUT: {
                if(p < 0.5) {
                    return 0.5 * (1 - sqrt(1 - 4 * (p * p)));
                }
                else {
                    return 0.5 * (sqrt(-((2 * p) - 3) * ((2 * p) - 1)) + 1);
                }
            }

            // Modeled after the exponential function y = 2^(10(x - 1))
            case TYPE::EXPOIN: {
                return (p == 0.0) ? p : pow(2, 10 * (p - 1));
            }

            // Modeled after the exponential function y = -2^(-10x) + 1
            case TYPE::EXPOOUT: {
                return (p == 1.0) ? p : 1 - pow(2, -10 * p);
            }

            // Modeled after the piecewise exponential
            // y = (1/2)2^(10(2x - 1))         ; [0,0.5)
            // y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
            case TYPE::EXPOINOUT: {
                if(p == 0.0 || p == 1.0) return p;
                
                if(p < 0.5) {
                    return 0.5 * pow(2, (20 * p) - 10);
                }
                else {
                    return -0.5 * pow(2, (-20 * p) + 10) + 1;
                }
            }

            // Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
            case TYPE::ELASTICIN: {
                return sin(13 * pi2 * p) * pow(2, 10 * (p - 1));
            }

            // Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
            case TYPE::ELASTICOUT: {
                return sin(-13 * pi2 * (p + 1)) * pow(2, -10 * p) + 1;
            }

            // Modeled after the piecewise exponentially-damped sine wave:
            // y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
            // y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
            case TYPE::ELASTICINOUT: {
                if(p < 0.5) {
                    return 0.5 * sin(13 * pi2 * (2 * p)) * pow(2, 10 * ((2 * p) - 1));
                }
                else {
                    return 0.5 * (sin(-13 * pi2 * ((2 * p - 1) + 1)) * pow(2, -10 * (2 * p - 1)) + 2);
                }
            }

            // Modeled (originally) after the overshooting cubic y = x^3-x*sin(x*pi)
            case TYPE::BACKIN: { /*
                return p * p * p - p * sin(p * pi); */
                double s = 1.70158f;
                return p * p * ((s + 1) * p - s);
            }

            // Modeled (originally) after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
            case TYPE::BACKOUT: { /*
                double f = (1 - p);
                return 1 - (f * f * f - f * sin(f * pi)); */
                double s = 1.70158f;
                return --p, 1.f * (p*p*((s+1)*p + s) + 1);
            }

            // Modeled (originally) after the piecewise overshooting cubic function:
            // y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
            // y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
            case TYPE::BACKINOUT: { /*
                if(p < 0.5) {
                    double f = 2 * p;
                    return 0.5 * (f * f * f - f * sin(f * pi));
                }
                else {
                    double f = (1 - (2*p - 1));
                    return 0.5 * (1 - (f * f * f - f * sin(f * pi))) + 0.5;
                } */
                double s = 1.70158f * 1.525f;
                if (p < 0.5) {
                    return p *= 2, 0.5 * p * p * (p*s+p-s);
                }
                else {
                    return p = p * 2 - 2, 0.5 * (2 + p*p*(p*s+p+s));
                }
            }

#           define tween$bounceout(p) ( \
                (p) < 4/11.0 ? (121 * (p) * (p))/16.0 : \
                (p) < 8/11.0 ? (363/40.0 * (p) * (p)) - (99/10.0 * (p)) + 17/5.0 : \
                (p) < 9/10.0 ? (4356/361.0 * (p) * (p)) - (35442/1805.0 * (p)) + 16061/1805.0 \
                           : (54/5.0 * (p) * (p)) - (513/25.0 * (p)) + 268/25.0 )

            case TYPE::BOUNCEIN: {
                return 1 - tween$bounceout(1 - p);
            }

            case TYPE::BOUNCEOUT: {
                return tween$bounceout(p);
            }

            case TYPE::BOUNCEINOUT: {
                if(p < 0.5) {
                    return 0.5 * (1 - tween$bounceout(1 - p * 2));
                }
                else {
                    return 0.5 * tween$bounceout((p * 2 - 1)) + 0.5;
                }
            }

#           undef tween$bounceout

            case TYPE::SINESQUARE: {
                double A = sin((p)*pi2);
                return A*A;
            }

            case TYPE::EXPONENTIAL: {
                return 1/(1+exp(6-12*(p)));                
            }

            case TYPE::SCHUBRING1: {
                return 2*(p+(0.5f-p)*abs(0.5f-p))-0.5f;                
            }

            case TYPE::SCHUBRING2: {
                double p1pass= 2*(p+(0.5f-p)*abs(0.5f-p))-0.5f;
                double p2pass= 2*(p1pass+(0.5f-p1pass)*abs(0.5f-p1pass))-0.5f;
                double pAvg=(p1pass+p2pass)/2;
                return pAvg;
            }

            case TYPE::SCHUBRING3: {
                double p1pass= 2*(p+(0.5f-p)*abs(0.5f-p))-0.5f;
                double p2pass= 2*(p1pass+(0.5f-p1pass)*abs(0.5f-p1pass))-0.5f;
                return p2pass;
            }

            case TYPE::SWING: {
                return ((-cos(pi * p) * 0.5) + 0.5);
            }

            case TYPE::SINPI2: {
                return sin(p * pi2);
            }
        }
    }

#   define $tween_xmacro(...) \
    $tween( undefined, UNDEFINED ) \
    \
    $tween( linear, LINEAR ) \
    $tween( quadin, QUADIN ) \
    $tween( quadout, QUADOUT ) \
    $tween( quadinout, QUADINOUT ) \
    $tween( cubicin, CUBICIN ) \
    $tween( cubicout, CUBICOUT ) \
    $tween( cubicinout, CUBICINOUT ) \
    $tween( quartin, QUARTIN ) \
    $tween( quartout, QUARTOUT ) \
    $tween( quartinout, QUARTINOUT ) \
    $tween( quintin, QUINTIN ) \
    $tween( quintout, QUINTOUT ) \
    $tween( quintinout, QUINTINOUT ) \
    $tween( sinein, SINEIN ) \
    $tween( sineout, SINEOUT ) \
    $tween( sineinout, SINEINOUT ) \
    $tween( expoin, EXPOIN ) \
    $tween( expoout, EXPOOUT ) \
    $tween( expoinout, EXPOINOUT ) \
    $tween( circin, CIRCIN ) \
    $tween( circout, CIRCOUT ) \
    $tween( circinout, CIRCINOUT ) \
    $tween( elasticin, ELASTICIN ) \
    $tween( elasticout, ELASTICOUT ) \
    $tween( elasticinout, ELASTICINOUT ) \
    $tween( backin, BACKIN ) \
    $tween( backout, BACKOUT ) \
    $tween( backinout, BACKINOUT ) \
    $tween( bouncein, BOUNCEIN ) \
    $tween( bounceout, BOUNCEOUT ) \
    $tween( bounceinout, BOUNCEINOUT ) \
    \
    $tween( sinesquare, SINESQUARE ) \
    $tween( exponential, EXPONENTIAL ) \
    \
    $tween( terrys1, SCHUBRING1 ) \
    $tween( terrys2, SCHUBRING2 ) \
    $tween( terrys3, SCHUBRING3 ) \
    \
    $tween( swing, SWING ) \
    $tween( sinpi2, SINPI2 ) 

    // interface for tweeners; includes memoization
#   define $tween(fn,type) \
    static inline double fn( double dt01 ) { \
        return tween::ease( tween::type, dt01 ); \
    }
    $tween_xmacro(expand functions)
#   undef $tween

    static inline
    const char *nameof( int type ) {
#   define $tween( unused, type ) case TYPE::type: return #type;
        switch(type) { 
            default:
            $tween_xmacro(expand cases)
        }
#   undef $tween
    }

    static inline
    double ping( double dt01 ) {
        return dt01;
    }
    static inline
    double pong( double dt01 ) {
        return 1 - dt01;
    }
    static inline
    double pingpong( double dt01 ) {
        return dt01 < 0.5f ? dt01 + dt01 : 2 - dt01 - dt01;
    }
    static inline
    double pongping( double dt01 ) {
        return 1 - (dt01 < 0.5f ? dt01 + dt01 : 2 - dt01 - dt01);
    }

    template<typename EASE>
    static inline double in( double dt01, EASE &ease ) { 
        return ease( dt01 );
    }
    template<typename EASE>
    static inline double out( double dt01, EASE &ease ) {
        return 1 - ease( 1 - dt01 );
    }
    template<typename EASE>
    static inline double inout( double dt01, EASE &ease ) { 
        return dt01 < 0.5f ? ease(dt01*2) * 0.5f : out(dt01*2-1, ease) * 0.5f + 0.5f;
    }
}
