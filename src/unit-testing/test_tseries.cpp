/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
/*
 *  test_tseries.cpp
 *  hector
 *
 *  Created by d3x290-local on 9/23/10.
 *  Copyright 2010 DOE Pacific Northwest Lab. All rights reserved.
 *
 */

#include <iostream>
#include <gtest/gtest.h>

#include "tseries.hpp"
#include "h_exception.hpp"

using namespace std;

TEST(TSeriesTest, SmallBasics) {
    
    Hector::tseries<double> test;
	
    // Simple tests
    
    double v1 = 2, v2 = 4;
    int p1 = 1, p2 = 2;
    test.set( p1, v1 );		
    test.set( p2, v2 );
    
    EXPECT_EQ( test.get( p1 ), v1 );
    EXPECT_EQ( test.get( p2 ), v2 );
    
    v1 = 5.0;
    test.set( p1, v1 );
    
    EXPECT_EQ( test.get( p1 ), v1 );
}

TEST(TSeriesTest, SmallFirstLast) {

	Hector::tseries<double> test;
	
	int p1 = 1, p2 = 2, p3 = 3;
    test.set( p1, 2 );		
    test.set( p2, 4 );
    test.set( p3, 6 );

    EXPECT_EQ( test.firstdate(),p1 );
    EXPECT_EQ( test.lastdate(), p3 );
}

TEST(TSeriesTest, SmallLinearInterp) {

    // Interpolation testing

	Hector::tseries<double> test;
	int p1 = 1, p2 = 2;
	double v1 = 4, v2 = 5;
    test.set( p1, v1 );		
    test.set( p2, v2 );
    test.allowInterp( true );
    
	// out-of-range interpolation requests should return end value
    EXPECT_EQ( test.get( p1 ), v1 );
    EXPECT_EQ( test.get( p2 ), v2 );

    EXPECT_EQ( ( v1+v2 )/2, test.get( ( p1+p2 )/2.0 ) );

	for( double i=0.0; i<=1.0; i+=0.1 )
		EXPECT_EQ( v1+(v2-v1)*i, test.get( p1+(p2-p1)*i ) );
    
}

TEST(TSeriesTest, EmptyTS) {
	
	Hector::tseries<double> test_empty;
    test_empty.allowInterp( true );
	
	EXPECT_EQ( test_empty.size(), 0 );
    EXPECT_THROW( test_empty.get( 1 ), h_exception );
}

TEST(TSeriesTest, NoInterpAllowed) {
	
	// No interpolation allowed case
	Hector::tseries<double> test;
    test.allowInterp(false);
    EXPECT_THROW( test.get( 4234.0 ), h_exception );
}

TEST(TSeriesTest, SmallOverwrite) {
	
	Hector::tseries<double> test;
	int p1 = 1, p2 = 2;
	double v1 = 4, v2 = 5;
    test.set( p1, v1 );		
    test.set( p2, v2 );
    EXPECT_EQ( test.get( p1 ), v1 );
    test.set( p1, v2 );
    EXPECT_EQ( test.get( p1 ), v2 );

}

TEST(TSeriesTest, BigTimeSeries) {

    // Test with a big time series

    // Monthly mean atmospheric carbon dioxide concentrations observed
    // at Mauna Loa Observatory, Hawaii, 1959 - 2008
    
    double x[] = {
        1959.042, 1959.125, 1959.208, 1959.292, 1959.375, 1959.458, 1959.542,
        1959.625, 1959.708, 1959.792, 1959.875, 1959.958, 1960.042, 1960.125,
        1960.208, 1960.292, 1960.375, 1960.458, 1960.542, 1960.625, 1960.708,
        1960.792, 1960.875, 1960.958, 1961.042, 1961.125, 1961.208, 1961.292,
        1961.375, 1961.458, 1961.542, 1961.625, 1961.708, 1961.792, 1961.875,
        1961.958, 1962.042, 1962.125, 1962.208, 1962.292, 1962.375, 1962.458,
        1962.542, 1962.625, 1962.708, 1962.792, 1962.875, 1962.958, 1963.042,
        1963.125, 1963.208, 1963.292, 1963.375, 1963.458, 1963.542, 1963.625,
        1963.708, 1963.792, 1963.875, 1963.958, 1964.042, 1964.125, 1964.208,
        1964.292, 1964.375, 1964.458, 1964.542, 1964.625, 1964.708, 1964.792,
        1964.875, 1964.958, 1965.042, 1965.125, 1965.208, 1965.292, 1965.375,
        1965.458, 1965.542, 1965.625, 1965.708, 1965.792, 1965.875, 1965.958,
        1966.042, 1966.125, 1966.208, 1966.292, 1966.375, 1966.458, 1966.542,
        1966.625, 1966.708, 1966.792, 1966.875, 1966.958, 1967.042, 1967.125,
        1967.208, 1967.292, 1967.375, 1967.458, 1967.542, 1967.625, 1967.708,
        1967.792, 1967.875, 1967.958, 1968.042, 1968.125, 1968.208, 1968.292,
        1968.375, 1968.458, 1968.542, 1968.625, 1968.708, 1968.792, 1968.875,
        1968.958, 1969.042, 1969.125, 1969.208, 1969.292, 1969.375, 1969.458,
        1969.542, 1969.625, 1969.708, 1969.792, 1969.875, 1969.958, 1970.042,
        1970.125, 1970.208, 1970.292, 1970.375, 1970.458, 1970.542, 1970.625,
        1970.708, 1970.792, 1970.875, 1970.958, 1971.042, 1971.125, 1971.208,
        1971.292, 1971.375, 1971.458, 1971.542, 1971.625, 1971.708, 1971.792,
        1971.875, 1971.958, 1972.042, 1972.125, 1972.208, 1972.292, 1972.375,
        1972.458, 1972.542, 1972.625, 1972.708, 1972.792, 1972.875, 1972.958,
        1973.042, 1973.125, 1973.208, 1973.292, 1973.375, 1973.458, 1973.542,
        1973.625, 1973.708, 1973.792, 1973.875, 1973.958, 1974.042, 1974.125,
        1974.208, 1974.292, 1974.375, 1974.458, 1974.542, 1974.625, 1974.708,
        1974.792, 1974.875, 1974.958, 1975.042, 1975.125, 1975.208, 1975.292,
        1975.375, 1975.458, 1975.542, 1975.625, 1975.708, 1975.792, 1975.875,
        1975.958, 1976.042, 1976.125, 1976.208, 1976.292, 1976.375, 1976.458,
        1976.542, 1976.625, 1976.708, 1976.792, 1976.875, 1976.958, 1977.042,
        1977.125, 1977.208, 1977.292, 1977.375, 1977.458, 1977.542, 1977.625,
        1977.708, 1977.792, 1977.875, 1977.958, 1978.042, 1978.125, 1978.208,
        1978.292, 1978.375, 1978.458, 1978.542, 1978.625, 1978.708, 1978.792,
        1978.875, 1978.958, 1979.042, 1979.125, 1979.208, 1979.292, 1979.375,
        1979.458, 1979.542, 1979.625, 1979.708, 1979.792, 1979.875, 1979.958,
        1980.042, 1980.125, 1980.208, 1980.292, 1980.375, 1980.458, 1980.542,
        1980.625, 1980.708, 1980.792, 1980.875, 1980.958, 1981.042, 1981.125,
        1981.208, 1981.292, 1981.375, 1981.458, 1981.542, 1981.625, 1981.708,
        1981.792, 1981.875, 1981.958, 1982.042, 1982.125, 1982.208, 1982.292,
        1982.375, 1982.458, 1982.542, 1982.625, 1982.708, 1982.792, 1982.875,
        1982.958, 1983.042, 1983.125, 1983.208, 1983.292, 1983.375, 1983.458,
        1983.542, 1983.625, 1983.708, 1983.792, 1983.875, 1983.958, 1984.042,
        1984.125, 1984.208, 1984.292, 1984.375, 1984.458, 1984.542, 1984.625,
        1984.708, 1984.792, 1984.875, 1984.958, 1985.042, 1985.125, 1985.208,
        1985.292, 1985.375, 1985.458, 1985.542, 1985.625, 1985.708, 1985.792,
        1985.875, 1985.958, 1986.042, 1986.125, 1986.208, 1986.292, 1986.375,
        1986.458, 1986.542, 1986.625, 1986.708, 1986.792, 1986.875, 1986.958,
        1987.042, 1987.125, 1987.208, 1987.292, 1987.375, 1987.458, 1987.542,
        1987.625, 1987.708, 1987.792, 1987.875, 1987.958, 1988.042, 1988.125,
        1988.208, 1988.292, 1988.375, 1988.458, 1988.542, 1988.625, 1988.708,
        1988.792, 1988.875, 1988.958, 1989.042, 1989.125, 1989.208, 1989.292,
        1989.375, 1989.458, 1989.542, 1989.625, 1989.708, 1989.792, 1989.875,
        1989.958, 1990.042, 1990.125, 1990.208, 1990.292, 1990.375, 1990.458,
        1990.542, 1990.625, 1990.708, 1990.792, 1990.875, 1990.958, 1991.042,
        1991.125, 1991.208, 1991.292, 1991.375, 1991.458, 1991.542, 1991.625,
        1991.708, 1991.792, 1991.875, 1991.958, 1992.042, 1992.125, 1992.208,
        1992.292, 1992.375, 1992.458, 1992.542, 1992.625, 1992.708, 1992.792,
        1992.875, 1992.958, 1993.042, 1993.125, 1993.208, 1993.292, 1993.375,
        1993.458, 1993.542, 1993.625, 1993.708, 1993.792, 1993.875, 1993.958,
        1994.042, 1994.125, 1994.208, 1994.292, 1994.375, 1994.458, 1994.542,
        1994.625, 1994.708, 1994.792, 1994.875, 1994.958, 1995.042, 1995.125,
        1995.208, 1995.292, 1995.375, 1995.458, 1995.542, 1995.625, 1995.708,
        1995.792, 1995.875, 1995.958, 1996.042, 1996.125, 1996.208, 1996.292,
        1996.375, 1996.458, 1996.542, 1996.625, 1996.708, 1996.792, 1996.875,
        1996.958, 1997.042, 1997.125, 1997.208, 1997.292, 1997.375, 1997.458,
        1997.542, 1997.625, 1997.708, 1997.792, 1997.875, 1997.958, 1998.042,
        1998.125, 1998.208, 1998.292, 1998.375, 1998.458, 1998.542, 1998.625,
        1998.708, 1998.792, 1998.875, 1998.958, 1999.042, 1999.125, 1999.208,
        1999.292, 1999.375, 1999.458, 1999.542, 1999.625, 1999.708, 1999.792,
        1999.875, 1999.958, 2000.042, 2000.125, 2000.208, 2000.292, 2000.375,
        2000.458, 2000.542, 2000.625, 2000.708, 2000.792, 2000.875, 2000.958,
        2001.042, 2001.125, 2001.208, 2001.292, 2001.375, 2001.458, 2001.542,
        2001.625, 2001.708, 2001.792, 2001.875, 2001.958, 2002.042, 2002.125,
        2002.208, 2002.292, 2002.375, 2002.458, 2002.542, 2002.625, 2002.708,
        2002.792, 2002.875, 2002.958, 2003.042, 2003.125, 2003.208, 2003.292,
        2003.375, 2003.458, 2003.542, 2003.625, 2003.708, 2003.792, 2003.875,
        2003.958, 2004.042, 2004.125, 2004.208, 2004.292, 2004.375, 2004.458,
        2004.542, 2004.625, 2004.708, 2004.792, 2004.875, 2004.958, 2005.042,
        2005.125, 2005.208, 2005.292, 2005.375, 2005.458, 2005.542, 2005.625,
        2005.708, 2005.792, 2005.875, 2005.958, 2006.042, 2006.125, 2006.208,
        2006.292, 2006.375, 2006.458, 2006.542, 2006.625, 2006.708, 2006.792,
        2006.875, 2006.958, 2007.042, 2007.125, 2007.208, 2007.292, 2007.375,
        2007.458, 2007.542, 2007.625, 2007.708, 2007.792, 2007.875, 2007.958,
        2008.042, 2008.125, 2008.208, 2008.292, 2008.375, 2008.458, 2008.542,
        2008.625, 2008.708, 2008.792, 2008.875, 2008.958
    };
    
    double y[] = {
        315.62, 316.38, 316.71, 317.72, 318.29, 318.16, 316.55, 314.80, 313.84,
        313.26, 314.80, 315.59, 316.43, 316.97, 317.58, 319.02, 320.02, 319.59,
        318.18, 315.91, 314.16, 313.83, 315.00, 316.19, 316.93, 317.70, 318.54,
        319.48, 320.58, 319.77, 318.58, 316.79, 314.80, 315.38, 316.10, 317.01,
        317.94, 318.55, 319.68, 320.63, 321.01, 320.55, 319.58, 317.40, 316.26,
        315.42, 316.69, 317.70, 318.74, 319.08, 319.86, 321.39, 322.24, 321.47,
        319.74, 317.77, 316.21, 315.99, 317.12, 318.31, 319.57, 320.11, 320.76,
        321.79, 322.24, 321.89, 320.44, 318.70, 316.70, 316.79, 317.79, 318.71,
        319.44, 320.44, 320.89, 322.13, 322.16, 321.87, 321.39, 318.80, 317.81,
        317.30, 318.87, 319.42, 320.62, 321.59, 322.39, 323.87, 324.01, 323.75,
        322.40, 320.37, 318.64, 318.10, 319.78, 321.08, 322.06, 322.50, 323.04,
        324.42, 325.00, 324.09, 322.55, 320.92, 319.31, 319.31, 320.72, 321.96,
        322.57, 323.15, 323.89, 325.02, 325.57, 325.36, 324.14, 322.03, 320.41,
        320.25, 321.31, 322.84, 324.00, 324.42, 325.64, 326.66, 327.34, 326.76,
        325.88, 323.67, 322.38, 321.78, 322.85, 324.12, 325.03, 325.99, 326.87,
        328.14, 328.07, 327.66, 326.35, 324.69, 323.10, 323.16, 323.98, 325.13,
        326.17, 326.68, 327.18, 327.78, 328.92, 328.57, 327.34, 325.46, 323.36,
        323.56, 324.80, 326.01, 326.77, 327.63, 327.75, 329.72, 330.07, 329.09,
        328.05, 326.32, 324.93, 325.06, 326.50, 327.55, 328.55, 329.56, 330.30,
        331.50, 332.48, 332.07, 330.87, 329.31, 327.51, 327.18, 328.16, 328.64,
        329.35, 330.71, 331.48, 332.65, 333.15, 332.13, 330.99, 329.17, 327.41,
        327.21, 328.34, 329.50, 330.68, 331.41, 331.85, 333.29, 333.91, 333.40,
        331.74, 329.88, 328.57, 328.35, 329.33, 330.55, 331.66, 332.75, 333.46,
        334.78, 334.79, 334.05, 332.95, 330.64, 328.96, 328.77, 330.18, 331.65,
        332.69, 333.23, 334.97, 336.03, 336.82, 336.10, 334.79, 332.53, 331.19,
        331.21, 332.35, 333.47, 335.09, 335.26, 336.62, 337.77, 338.00, 337.98,
        336.48, 334.37, 332.33, 332.40, 333.76, 334.83, 336.21, 336.64, 338.13,
        338.96, 339.02, 339.20, 337.60, 335.56, 333.93, 334.12, 335.26, 336.78,
        337.80, 338.28, 340.04, 340.86, 341.47, 341.26, 339.34, 337.45, 336.10,
        336.05, 337.21, 338.29, 339.36, 340.51, 341.57, 342.56, 343.01, 342.52,
        340.71, 338.51, 336.96, 337.13, 338.58, 339.91, 340.92, 341.69, 342.87,
        343.83, 344.30, 343.42, 341.85, 339.82, 337.98, 338.09, 339.24, 340.67,
        341.42, 342.67, 343.45, 345.08, 345.75, 345.32, 343.93, 342.08, 340.00,
        340.12, 341.35, 342.89, 343.87, 344.59, 345.29, 346.58, 347.36, 346.80,
        345.37, 343.06, 341.24, 341.54, 342.90, 344.36, 345.08, 345.89, 347.49,
        348.02, 348.75, 348.19, 346.49, 344.70, 343.04, 342.92, 344.22, 345.61,
        346.42, 346.95, 347.88, 349.57, 350.35, 349.70, 347.78, 345.89, 344.88,
        344.34, 345.67, 346.89, 348.20, 348.55, 349.56, 351.12, 351.84, 351.45,
        349.77, 347.62, 346.37, 346.48, 347.80, 349.03, 350.23, 351.58, 352.22,
        353.53, 354.14, 353.64, 352.53, 350.42, 348.84, 348.94, 349.99, 351.29,
        352.72, 353.10, 353.64, 355.43, 355.70, 355.11, 353.79, 351.42, 349.83,
        350.10, 351.26, 352.66, 353.63, 354.72, 355.49, 356.10, 357.08, 356.11,
        354.67, 352.67, 351.05, 351.36, 352.81, 354.21, 354.87, 355.67, 357.00,
        358.40, 359.00, 357.99, 355.96, 353.78, 352.20, 352.22, 353.70, 354.98,
        356.08, 356.84, 357.73, 358.91, 359.45, 359.19, 356.72, 354.77, 352.80,
        353.21, 354.15, 355.39, 356.76, 357.17, 358.26, 359.17, 360.07, 359.41,
        357.36, 355.29, 353.96, 354.03, 355.27, 356.70, 358.05, 358.80, 359.67,
        361.13, 361.48, 360.60, 359.20, 357.23, 355.42, 355.89, 357.41, 358.74,
        359.73, 360.61, 361.60, 363.05, 363.62, 363.03, 361.55, 358.94, 357.93,
        357.80, 359.22, 360.42, 361.83, 362.94, 363.91, 364.28, 364.93, 364.70,
        363.31, 361.15, 359.41, 359.34, 360.62, 361.96, 362.81, 363.87, 364.25,
        366.02, 366.47, 365.36, 364.10, 361.89, 360.05, 360.49, 362.21, 364.12,
        365.00, 365.82, 366.95, 368.42, 369.33, 368.78, 367.59, 365.81, 363.83,
        364.18, 365.36, 366.87, 367.97, 368.83, 369.46, 370.77, 370.66, 370.10,
        369.10, 366.70, 364.61, 365.17, 366.51, 367.86, 369.07, 369.32, 370.38,
        371.63, 371.32, 371.51, 369.69, 368.18, 366.87, 366.94, 368.27, 369.62,
        370.47, 371.44, 372.39, 373.32, 373.77, 373.13, 371.51, 369.59, 368.12,
        368.38, 369.64, 371.11, 372.38, 373.08, 373.87, 374.93, 375.58, 375.44,
        373.91, 371.77, 370.72, 370.50, 372.19, 373.71, 374.92, 375.63, 376.51,
        377.75, 378.54, 378.21, 376.65, 374.28, 373.12, 373.10, 374.67, 375.97,
        377.03, 377.87, 378.88, 380.42, 380.62, 379.66, 377.48, 376.07, 374.10,
        374.47, 376.15, 377.51, 378.43, 379.70, 380.91, 382.20, 382.45, 382.14,
        380.60, 378.60, 376.72, 376.98, 378.29, 380.07, 381.36, 382.19, 382.65,
        384.65, 384.94, 384.01, 382.15, 380.33, 378.81, 379.06, 380.17, 381.85,
        382.88, 383.77, 384.42, 386.36, 386.53, 386.01, 384.45, 381.96, 380.81,
        381.09, 382.37, 383.84, 385.42, 385.72, 385.96, 387.18, 388.50, 387.88,
        386.38, 384.15, 383.07, 382.98, 384.11, 385.54
    };
    
    int n = sizeof( x ) / sizeof( double );
    
    Hector::tseries<double> co2;
    co2.allowInterp( true );    
    int i;
    for( i=0; i<n; i++)
        co2.set( x[ i ], y[ i ] );

	// Does size() work as expected?
    EXPECT_EQ( co2.size(), n ) << "Testing size";

	// Does exists() work?
    for( int i=0; i<n; i++) {
        EXPECT_TRUE( co2.exists( x[ i ] ) );
        EXPECT_FALSE( co2.exists( -x[ i ] ) );
    }
}

TEST(TSeriesTest, PartialInterp) {
	Hector::tseries<double> test;
    test.set( 1, 1 );
    test.set( 2, 2 );
    test.allowPartialInterp( true );
    EXPECT_THROW( test.get( 3 ), h_exception );
    EXPECT_NO_THROW( test.get( 1.5 ) );
}
