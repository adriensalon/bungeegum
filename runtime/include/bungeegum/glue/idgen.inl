#pragma once

namespace bungeegum {
namespace detail {

    template <typename instance_t, typename id_t>
    id_t id_generator<instance_t, id_t>::generate()
    {
        id_t _new_id;
        if (_to_recycle.empty()) {
            _new_id = _limit;
            _limit++;
        } else {
            _new_id = _to_recycle.back();
            _to_recycle.pop();
        }
        return _new_id;
    }

    template <typename instance_t, typename id_t>
    void id_generator<instance_t, id_t>::recycle(const id_t id)
    {
        _to_recycle.push(id);
    }

    template <typename instance_t, typename id_t>
    id_t id_generator_concurrent<instance_t, id_t>::generate()
    {
        id_t _new_id;
        if (_to_recycle.empty()) {
            _new_id = _limit.load();
            _limit.store(_limit + 1);
        } else {
            _new_id = _to_recycle.back();
            _to_recycle.pop();
        }
        return _new_id;
    }

    template <typename instance_t, typename id_t>
    void id_generator_concurrent<instance_t, id_t>::recycle(const id_t id)
    {
        _to_recycle.push(id);
    }

}
}
