/*
 * MikroSDK - MikroE Software Development Kit
 * Copyright© 2020 MikroElektronika d.o.o.
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 * OR OTHER DEALINGS IN THE SOFTWARE. 
 */

/*!
 * \file
 *
 */
#include "configuration.h"
#include "../include/multimeter.h"

// ------------------------------------------------------------- PRIVATE MACROS 

#define MULTIMETER_DUMMY 0

// ------------------------------------------------------------------ CONSTANTS

static const float resolution_select[ 6 ] = { 100.0, 1000.0, 10000.0,
                                              100000.0, 470000.0, 1000000.0 };

// ---------------------------------------------- PRIVATE FUNCTION DECLARATIONS 

static uint16_t drv_read_channel ( multimeter_t *ctx, uint8_t address );

static void drv_calibrate_capacitance ( multimeter_t *ctx );

static void drv_calibrate_voltage ( multimeter_t *ctx );

static void drv_calibrate_current ( multimeter_t *ctx );

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void multimeter_cfg_setup ( multimeter_cfg_t *cfg )
{

    // Additional gpio pins

    cfg->a = GPIOA_REGS->GPIO_LAT;
    cfg->b = GPIOB_REGS->GPIO_LAT;
    cfg->c = GPIOB_REGS->GPIO_LAT;


    // Calibration config. 

    cfg->calibration_cfg.capa_cal_cfg = 0;
    cfg->calibration_cfg.volt_cal_cfg = 0;
    cfg->calibration_cfg.curr_cal_cfg = 0;
    cfg->calibration_cfg.scan_range_cfg = 0;
}

MULTIMETER_RETVAL multimeter_init ( multimeter_t *ctx, multimeter_cfg_t *cfg )
{

    // Calibration structure

    ctx->calibration.capacitance_cal = cfg->calibration_cfg.capa_cal_cfg;
    ctx->calibration.voltage_cal = cfg->calibration_cfg.volt_cal_cfg;
    ctx->calibration.current_cal = cfg->calibration_cfg.curr_cal_cfg;
    ctx->calibration.scan_range = cfg->calibration_cfg.scan_range_cfg;

    return MULTIMETER_OK;
}

void multimeter_default_cfg ( multimeter_t *ctx )
{
    drv_calibrate_capacitance( ctx );
    drv_calibrate_voltage( ctx );
    drv_calibrate_current( ctx ); 
}

float multimeter_read_capacitance ( multimeter_t *ctx )
{
    float value;

    value = ( float ) drv_read_channel( ctx, MULTIMETER_C_CHANNEL ) * 2;
    value = (  64285 / value - ctx->calibration.capacitance_cal ) * 2;

    if ( value < 1 )
    {
        return 0;
    }

    return value;
}

float multimeter_read_voltage ( multimeter_t *ctx )
{
    float value;

    value = ( float ) drv_read_channel( ctx, MULTIMETER_U_CHANNEL ) / 2;
    value = ( value - ctx->calibration.voltage_cal ) * 33 / 2;

    return value;
}

float multimeter_read_current ( multimeter_t *ctx )
{
    float value;

    value = ( float ) drv_read_channel( ctx, MULTIMETER_I_CHANNEL ) / 2;
    value -= ctx->calibration.current_cal;

    return value;
}

float multimeter_read_resistance ( multimeter_t *ctx )
{
    float value;
    uint16_t range;

    range = multimeter_get_resistance_range( ctx, ctx->calibration.scan_range );

    if ( ctx->calibration.scan_range >= 5 )
    {
        ctx->calibration.scan_range = 0;
    }

    if ( range > 4090 )
    {
        value = ( float ) ( ( resolution_select[ ctx->calibration.scan_range ] *
                              MULTIMETER_MAX_VOLTAGE ) / range ) - 
                              resolution_select[ ctx->calibration.scan_range ];
        ctx->calibration.scan_range--;
        return value;
    }
    else if ( range < 100 && ctx->calibration.scan_range < 5 )
    {
        ctx->calibration.scan_range++;
    }

    value = ( float ) ( ( resolution_select[ ctx->calibration.scan_range ] *
                          MULTIMETER_MAX_VOLTAGE ) / range ) -
                          resolution_select[ ctx->calibration.scan_range ];
    return value;
}

uint16_t multimeter_get_resistance_range ( multimeter_t *ctx, uint8_t range )
{
    if ( range > 5 )
    {
        range = 5;
    }

    multimeter_set_resistance_range( ctx, range );
    //Delay_100ms( );

    return drv_read_channel( ctx, MULTIMETER_R_CHANNEL );
}

void multimeter_set_resistance_range ( multimeter_t *ctx, uint8_t range )
{
    ( &ctx->a = range & 0x01 );
    ( &ctx->b = ( range >> 1 ) & 0x01 );
    ( &ctx->c = ( range >> 2 ) & 0x01 );
    //Delay_100ms( );
}

// ----------------------------------------------- PRIVATE FUNCTION DEFINITIONS

static uint16_t drv_read_channel ( multimeter_t *ctx, uint8_t channel )
{
    uint16_t value;
    uint8_t write_data;
    uint8_t read_buf[ 2 ];

    write_data = 0x06;

    spi_master_select_device( ctx->chip_select );
    spi_master_write( &ctx->spi, &write_data, 1 );

    write_data = ( channel & 0x03 ) << 6;
    
    spi_master_set_default_write_data( &ctx->spi, write_data );
    spi_master_read( &ctx->spi, read_buf, 1 );
    
    write_data      = 0x00;
    
    spi_master_set_default_write_data( &ctx->spi, write_data );
    spi_master_read( &ctx->spi, read_buf + 1, 1 );
    spi_master_deselect_device( ctx->chip_select );  
    
    spi_master_set_default_write_data( &ctx->spi, MULTIMETER_DUMMY );
    value = read_buf[ 0 ] & 0x0F;
    value <<= 8;
    value |= read_buf[ 1 ];

    return value;
}

static void drv_calibrate_capacitance ( multimeter_t *ctx )
{
    ctx->calibration.capacitance_cal = 64285.0 /
                     ( drv_read_channel( ctx, MULTIMETER_C_CHANNEL ) * 2 );
}

static void drv_calibrate_voltage ( multimeter_t *ctx ) 
{
    ctx->calibration.voltage_cal = ( float ) 
                   drv_read_channel( ctx, MULTIMETER_U_CHANNEL ) / 2;
}

static void drv_calibrate_current ( multimeter_t *ctx )
{
    ctx->calibration.current_cal = ( float )
                   drv_read_channel( ctx, MULTIMETER_I_CHANNEL ) / 2;
}

// ------------------------------------------------------------------------- END

