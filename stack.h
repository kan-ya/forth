#pragma once

#include "error.h"

#include <limits.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>

typedef long long int cell;

#define STACK_SIZE 64
extern uint16_t SP, RP;
extern cell  Stack[STACK_SIZE];
extern cell RStack[STACK_SIZE];


function dummy_push;

function print_stack;

function print;
function emit;

errorCode push(cell val);
function pop;
function dup;
function swap;
function nip;
function rot;
function pick;
function roll;

function add;
function sub;
function mul;
function divi;

function not;
function and;
function or;
function xor;

function eq;
function neq;
function lt;
function lte;
function gt;
function gte;

function invert;

function fromR;
function toR;

function Rpop;
function Rdup;
function Rswap;

function depth;
function Rdepth;
