#ifndef _LFT_ALGORITHM_HPP
#define _LFT_ALGORITHM_HPP

namespace lft {

template<class T>
constexpr T const& clamp(T const& v, T const& lo, T const& hi)
{
    return v < lo ? lo : ((v > hi) ? hi : v);
}

} // namespace lft

#endif // _LFT_ALGORITHM_HPP
