#include <gtest/gtest.h>
#include <staticgui/staticgui.hpp>

TEST(glue, registry_wrong_type)
{
    EXPECT_STREQ("ok", "ok");
    // this tests _that_ the expected exception is thrown
    // EXPECT_THROW({
    //     try
    //     {
    //         thisShallThrow();
    //     }
    //     catch( const MyException& e )
    //     {
    //         // and this tests that it has the correct message
    //         EXPECT_STREQ( "Cucumber overflow", e.what() );
    //         throw;
    //     }
    // }, MyException );
}