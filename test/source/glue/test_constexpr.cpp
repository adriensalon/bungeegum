//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <bungeegum/bungeegum.hpp>
#include <gtest/gtest.h>


TEST(bungeegum_glue, constexpr_for)
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

TEST(bungeegum_glue, constexpr_foreach)
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