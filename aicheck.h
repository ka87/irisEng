#pragma once

/* Which behaviours are actually running.
 *
 * AI_pst_RunFunction receives a tAIFunction with no key on it, so a
 * trace there cannot say which behaviour it is about. The loader does
 * have the key, so it records the pairing as each function is built
 * and this prints the name of any watched behaviour when it runs.
 *
 * That answers the question a trace inside a behaviour cannot: whether
 * the behaviour is being evaluated at all. Set AICHK_ENABLED to 0 to
 * compile it away. */
#define AICHK_ENABLED 1

/* Behaviour keys to report, terminated by 0. Keep the list short --
 * a behaviour on a reflex track runs every frame. */
#define AICHK_WATCH  { 0x9E0015FE,   /* vala_et_faisca_vef_wait */           \
                       0x9E00164D,   /* vala_et_faisca_vef_activate */       \
                       0x9E002DF8,   /* vala_et_faisca_vef_exec_set_action */\
                       0x7100F56D,   /* BV_Active_Desactive_BV_In */         \
                       0x7100F571,   /* BV_Active_Desactive_BV_Out */        \
                       0x9E003C7B,   /* Interrrupteur_Sol_IS_reflex */       \
                       0 }

/* Report at most this many runs of each watched key, so a per-frame
 * track cannot flood the log. 0 means no limit. */
#define AICHK_MAX_PER_KEY  20

struct sAIFunction;

#if AICHK_ENABLED
void AICHK_NoteFunction(struct sAIFunction* fn, unsigned int key);
void AICHK_Running(struct sAIFunction* fn);
#else
#define AICHK_NoteFunction(f, k)   ((void)0)
#define AICHK_Running(f)           ((void)0)
#endif
