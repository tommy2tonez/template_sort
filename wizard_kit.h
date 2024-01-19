#ifndef __DG_TEMPLATE_SORT_H__
#define __DG_TEMPLATE_SORT_H__

#include <utility>
#include <algorithm>
#include <tuple>

namespace dg::wizard::sort{

    template <class CallBack, class First, class Second, class ...Args>
    static void insertion_sort(const CallBack& callback, First first, Second second, Args ...args){
        
        if constexpr(sizeof...(Args) == 0){
            callback(std::min(first, second), std::max(first, second));
        } else{
            auto cb_lambda = [=]<class ...AArgs>(AArgs ...aargs){
                callback(std::min(first, second), aargs...);
            };

            insertion_sort(cb_lambda, std::max(first, second), args...);
        }

    } 

    template <class CallBack, class First, class ...Args>
    static void template_sort(const CallBack& callback, First first, Args ...args){

        if constexpr(sizeof...(Args) == 0){
            callback(first);
        } else{

            auto cb_lambda  = [=]<class ...AArgs>(AArgs ...aargs){
                insertion_sort(callback, first, aargs...);
            };

            template_sort(cb_lambda, args...);
        }

    }

    template <class _Ty, size_t SZ_Arg>
    static void template_sort_arr(_Ty * first, const std::integral_constant<size_t, SZ_Arg>&){

        auto sort_cb    = [=]<class ...Args>(Args ...args){
            
            auto fwd_tup        = std::make_tuple(args...);
            const auto idx_seq  = std::make_index_sequence<sizeof...(Args)>{};

            [=]<class Tup, size_t ...IDX>(Tup&& tup, const std::index_sequence<IDX...>&){
                ((first[IDX]  = std::get<IDX>(tup)), ...);
            }(fwd_tup, idx_seq);

        };

        const auto idx_seq    = std::make_index_sequence<SZ_Arg>{};

        [=]<size_t ...IDX>(const std::index_sequence<IDX...>&){
            template_sort(sort_cb, first[IDX]...);
        }(idx_seq);

    }
}

namespace dg::wizard::utility{

    template <class T, size_t BIT_LENGTH>
    auto fast_lower_bound(T key, T * data, const std::integral_constant<size_t, BIT_LENGTH>) -> size_t{

        auto idx_seq    = std::make_index_sequence<BIT_LENGTH>{};
        auto rs         = size_t{0u};

        [&]<size_t ...IDX>(const std::index_sequence<IDX...>){
            (
                [&]{
                    (void) IDX;
                    constexpr auto BIT_IDX  = BIT_LENGTH - IDX - 1; 
                    rs <<= 1;
                    rs |= 1;
                    rs ^= T{data[rs << BIT_IDX] > key}; 
                }(), ...
            );
        }(idx_seq);

        return rs;
    }

    template <class T, class Filter, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    auto filter(T * first, T * last, T * dst, Filter filter) -> T *{

        for (auto it = first; it != last; ++it){
            (*dst) = *it;
            dst += filter(*it);
        }

        return dst;
    } 

    template <class T, class Cond, class Accum>
    auto sequential_group(T * first, T * last, T * dst, Cond cond, Accum accumulator) -> T *{

        *(dst++) = first;

        for (auto it = first + 1; it != last; ++it){
            *dst = *it;
            dst += cond(*dst, *std::prev(dst));
            *std::prev(dst) = accumulator(*std::prev(dst), *it);
        }

        return std::prev(dst);
    } 
}

#endif
