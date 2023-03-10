/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
/*
 *  test_inih.cpp
 *  hector
 *
 *  Created by Ben on 2010-07-10
 *
 */

#include <iostream>
#include <gtest/gtest.h>
#include <fstream>

#include "INIReader.h"
#include "h_exception.hpp"

using namespace std;

// Small helper function for tests below - writes a test INI file
void test_inih_writefile(  char* filename, string contents ) {
    ofstream f( filename );
    if( f.is_open() ) {
        f << contents;
        f.close();
    }
	else {
		H_THROW( "Error writing test file " + string( filename ) );
	}
}

TEST( INIHTest, SimpleFile ) {

    char filename[] = "simple.ini";
    test_inih_writefile( filename, 
              "; Test config file for ini_test.c\n"
              "\n"
              "[Protocol]   ; Protocol configuration\n"
              "Version=6  ; IPv6\n"
              "\n"
              "[User]\n"
              "Name = Bob Smith       ; Spaces around '=' are stripped\n"
              "Email = bob@smith.com  ; And comments (like this) ignored\n" );
    
    Hector::INIReader reader( filename );
	EXPECT_FALSE( reader.ParseError() );
		
    EXPECT_EQ( reader.GetInteger( "protocol", "version", -1 ), 6 );
    EXPECT_EQ( reader.Get( "user", "name", "UNKNOWN" ), "Bob Smith" );
    EXPECT_EQ( reader.Get( "user", "email", "UNKNOWN" ), "bob@smith.com" );
    
    remove( filename );     // clean up
}

TEST( INIHTest, ComplexFile ) {

    char filename[] = "complex.ini";
    test_inih_writefile( filename, 
              "; This is an INI file\n"
              "[section1]  ; section comment\n"
              "one=This is a test  ; name=value comment\n"
              "two = 1234\n"
              "; x=y\n"
              "\n"
              "[ section 2 ]\n"
              "happy  =  4\n"
              "sad =\n"
              "\n"
              "[empty]\n"
              "; do nothing\n"
              "\n"
              "[comment_test]\n"
              "test1 = 1;2;3 ; only this will be a comment\n"
              "test2 = 2;3;4;this won't be a comment, needs whitespace before ';'\n"
              "test;3 = 345 ; key should be 'test;3'\n"
              "test4 = 4#5#6 ; '#' only starts a comment at start of line\n"
              "#test5 = 567 ; entire line commented\n"
              "# test6 = 678 ; entire line commented, except in MULTILINE mode\n" );
              
    Hector::INIReader reader( filename );
	EXPECT_FALSE( reader.ParseError() );
	
    EXPECT_EQ( reader.Get( "section1", "one", "" ), "This is a test" );
    EXPECT_EQ( reader.GetInteger( "section1", "two", -1 ), 1234 );
    EXPECT_NE( reader.Get( "section1", "x", "" ), "y" );
    
    EXPECT_EQ( reader.GetInteger( " section 2 ", "happy", -1 ), 4 );
    EXPECT_EQ( reader.Get( " section 2 ", "sad", "UNKNOWN" ), "" );
    EXPECT_NE( reader.GetInteger( " section 2 ", "sad", -1 ), 0 );

    EXPECT_EQ( reader.Get( "comment_test", "test1", "" ), "1;2;3" );
    EXPECT_EQ( reader.Get( "comment_test", "test2", "" ), 
            "2;3;4;this won't be a comment, needs whitespace before ';'" );
    EXPECT_EQ( reader.GetInteger( "comment_test", "test;3", -1 ), 345 );
    EXPECT_EQ( reader.Get( "comment_test", "test4", "" ), "4#5#6" );
    EXPECT_EQ( reader.Get( "comment_test", "test5", "UNKNOWN" ), "UNKNOWN" );
    EXPECT_EQ( reader.Get( "comment_test", "test6", "UNKNOWN" ), "UNKNOWN" );

    remove( filename );     // clean up    
}

TEST( INIHTest, CorruptSection ) {
    
    char filename[] = "corruptsection.ini";
    test_inih_writefile( filename, 
                "[section1]\n"
                "name1=value1\n"
                "[section2\n"
                "[section3   ; comment ]\n"
                "name2=value2\n" );
     
    Hector::INIReader reader( filename );
	EXPECT_TRUE( reader.ParseError() );
	
    remove( filename );     // clean up    
}
