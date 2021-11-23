/*
 * bp_custom.cpp
 * Computer Architecture - Lab 7: Branch Prediction
 * Derek Chiou
 * Alex Hsu, Chirag Sakhuja, Tommy Huynh
 * Spring 2016
 *
 * This file contains the implementation of the branch predictor.
 * YOU CAN CHANGE ANYTHING IN THIS FILE.
 * Implement a branch predictor of your own choice here. It starts off with the
 * same code as bp_btfn.cpp.
 */

#include "bp.h"
#include "bp_helper.h"


/*
 * Branch predictor state
 * Declare the global state needed by your branch predictor. All storage used
 * by your branch predictor should be here and cannot exceed 4 KiB.
 */
/*  Número de bits utilizados
    k = Número de bits no BHR
    a = Número de LSB do endereço do desvio utilizados no BHR ou PHT
        (em esquemas por endereço - p)
    m = Número de MSB do endereço do desvio utilizados no PHT
        (em esquemas por conjunto - s)
    i = Número de MSB do endereço do desvio utilizados no BHR
        (em esquemas por conjunto - s)
*/
int k = 6, i = 10, m = 4;

vector<BHR> bht;
vector<PHT> listaPht;

/*
 * Initialize branch predictor
 * This function is called once at the beginning of the program to initialize
 * the global branch predictor state.
 */
void BP::init()
{
    /* The following flag can be helpful when debugging your branch predictor.
    * A record of the branches and your predictions can be written to the file
    * branch_trace.out. The flag can be set to the following values:
    *   TRACE_LEVEL_NONE             - do not record any branches
    *   TRACE_LEVEL_WRONG_TARGETS    - record when targets are mispredicted
    *   TRACE_LEVEL_WRONG_DIRECTIONS - record when directions are mispredicted
    *   TRACE_LEVEL_ALL              - record all branches
    * Additionally, you can also write to this file by outputting to br_trace.
    */
    br_trace_level = TRACE_LEVEL_NONE;
    br_trace << "Custom Branch Predictor!" << endl;
    bht = criaBHT(i, k);
    listaPht = criaListaPHT(m, k);
}

/*
 * Predict branch
 * This function is called when a branch occurs and the argument struct
 * contains information used to predict the branch. This function returns a
 * struct that contains the prediction.
 * input:
 *   uintptr_t  br.inst_ptr      - program counter of the branch
 *   uintptr_t  br.next_inst_ptr - next program counter of the branch
 *   uintptr_t  br.target        - branch target if known (i.e. direct branch)
 *   bool       br.uncond        - true if branch is unconditional, else false
 *   bool       br.direct        - true if branch is direct, else false
 *   bool       br.call          - true if branch is a call instruction
 *   bool       br.ret           - true if branch is a return instruction
 *   string     br.dasm          - disassembly of instruction for debugging
 * output:
 *   bool       pred.taken       - predicted direction of the branch
 *                                   true = taken; false = not taken
 *   uintptr_t  pred.target      - predicted target of the branch
 *                                   this is the taken branch target regardless
 *                                   of predicted branch direction
 */
Prediction BP::predict(EntInfo br)
{
    bool taken;
    taken = listaPht[msb(br.target, m)].previsao(bht[msb(br.target, i)].conteudo);

    return Prediction(taken, br.target);
}

/*
 * Update branch predictor
 * This function is called when a branch resolves and the argument struct
 * contains information used to update the branch predictor.
 * input:
 *   uintptr_t  br.inst_ptr      - program counter of the branch
 *   uintptr_t  br.next_inst_ptr - next program counter of the branch
 *   uintptr_t  br.target        - actual target of the branch
 *   bool       br.taken         - actual direction of the branch
 *                                   true = taken; false = not taken
 *   bool       br.uncond        - true if branch is unconditional, else false
 *   bool       br.direct        - true if branch is direct, else false
 *   bool       br.call          - true if branch is a call instruction
 *   bool       br.ret           - true if branch is a return instruction
 *   string     br.dasm          - disassembly of instruction for debugging
 */
void BP::update(ResInfo br)
{
    listaPht[msb(br.target, m)].atualiza(bht[msb(br.target, i)].conteudo, br.taken);
    bht[msb(br.target, i)].atualiza(br.taken);
}
