#include <staticgui/staticgui.hpp>

struct A {

    A()
    {

        std::cout << reinterpret_cast<std::uintptr_t>(this) << std::endl;
    }
};

struct relou {
    int f = 44;
};

relou& okokoko()
{
    static relou _rel;

    std::cout << reinterpret_cast<std::uintptr_t>(&_rel) << std::endl;
    return _rel;
}

int main()
{
    std::cout << reinterpret_cast<std::uintptr_t>(&(okokoko())) << std::endl;

    // staticgui::launch(
    //     create<staticgui::widgets::container_widget>()
    //         .child(staticgui::widgets::container_widget()
    //                    .child(create<staticgui::widgets::container_widget>()
    //                               .child(staticgui::widgets::container_widget()))));

    staticgui::widgets::container_widget c1, c2;

    staticgui::launch(staticgui::create_widget<staticgui::widgets::container_widget>().ok().child(
        staticgui::create_widget<staticgui::widgets::container_widget>().ok().child(c2).ok()));
    // .child(staticgui::widgets::container_widget()
    //    .child(staticgui::widgets::container_widget()
    // .child(c2
    //            .child(c3)));

    return 0;
}