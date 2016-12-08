//****************************************************************************
// Model: game.qm
// File:  ./missile.cpp
//
// This code has been generated by QM tool (see state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//****************************************************************************
//${.::missile.cpp} ..........................................................
#include "qpcpp.h"
#include "bsp.h"
#include "game.h"

//Q_DEFINE_THIS_FILE

namespace GAME {

//${AOs::Missile} ............................................................
class Missile : public QP::QMActive {
private:
    uint8_t m_x;
    uint8_t m_y;
    uint8_t m_exp_ctr;

public:
    Missile();

protected:
    static QP::QState initial(Missile * const me, QP::QEvt const * const e);
    static QP::QState armed  (Missile * const me, QP::QEvt const * const e);
    static QP::QMState const armed_s;
    static QP::QState flying  (Missile * const me, QP::QEvt const * const e);
    static QP::QMState const flying_s;
    static QP::QState exploding  (Missile * const me, QP::QEvt const * const e);
    static QP::QState exploding_e(Missile * const me);
    static QP::QMState const exploding_s;
};

} // namespace GAME

namespace GAME {

// local objects -------------------------------------------------------------
static Missile l_missile; // the sole instance of the Missile active object

// Public-scope objects ------------------------------------------------------
QP::QMActive * const AO_Missile = &l_missile; // opaque pointer

} // namespace GAME

// Active object definition --------------------------------------------------
namespace GAME {

//${AOs::Missile} ............................................................
//${AOs::Missile::Missile} ...................................................
Missile::Missile()
  : QMActive(Q_STATE_CAST(&Missile::initial))
{}

//${AOs::Missile::SM} ........................................................
QP::QState Missile::initial(Missile * const me, QP::QEvt const * const e) {
    static QP::QMTranActTable const tatbl_ = { // transition-action table
        &armed_s,
        {
            Q_ACTION_CAST(0)  // zero terminator
        }
    };
    // ${AOs::Missile::SM::initial}
    me->subscribe( TIME_TICK_SIG);

    // object dictionary for Missile object
    QS_OBJ_DICTIONARY(&l_missile);

    // dictionaries for Missile SM...
    QS_FUN_DICTIONARY(&Missile::initial);
    QS_FUN_DICTIONARY(&Missile::armed);
    QS_FUN_DICTIONARY(&Missile::flying);
    QS_FUN_DICTIONARY(&Missile::exploding);

    // local signals...
    QS_SIG_DICTIONARY(MISSILE_FIRE_SIG,   &l_missile);
    QS_SIG_DICTIONARY(HIT_WALL_SIG,       &l_missile);
    QS_SIG_DICTIONARY(DESTROYED_MINE_SIG, &l_missile);

    (void)e; // unused parameter
    return QM_TRAN_INIT(&tatbl_);
}
//${AOs::Missile::SM::armed} .................................................
QP::QMState const Missile::armed_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&armed),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Missile::SM::armed}
QP::QState Missile::armed(Missile * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Missile::SM::armed::MISSILE_FIRE}
        case MISSILE_FIRE_SIG: {
            static QP::QMTranActTable const tatbl_ = { // transition-action table
                &flying_s,
                {
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            me->m_x = Q_EVT_CAST(ObjectPosEvt)->x;
            me->m_y = Q_EVT_CAST(ObjectPosEvt)->y;
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Missile::SM::flying} ................................................
QP::QMState const Missile::flying_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&flying),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Missile::SM::flying}
QP::QState Missile::flying(Missile * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Missile::SM::flying::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Missile::SM::flying::TIME_TICK::[me->m_x+GAME_MISSILE_SPEED_X<GA~}
            if (me->m_x + GAME_MISSILE_SPEED_X < GAME_TUNNEL_WIDTH) {
                me->m_x += GAME_MISSILE_SPEED_X;
                // tell the Tunnel to draw the Missile and test for wall hits
                ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, MISSILE_IMG_SIG);
                oie->x   = me->m_x;
                oie->y   = me->m_y;
                oie->bmp = MISSILE_BMP;
                AO_Tunnel->POST(oie, me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Missile::SM::flying::TIME_TICK::[else]}
            else {
                static QP::QMTranActTable const tatbl_ = { // transition-action table
                    &armed_s,
                    {
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        // ${AOs::Missile::SM::flying::HIT_WALL}
        case HIT_WALL_SIG: {
            static struct {
                QP::QMState const *target;
                QP::QActionHandler act[2];
            } const tatbl_ = { // transition-action table
                &exploding_s,
                {
                    Q_ACTION_CAST(&exploding_e), // entry
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        // ${AOs::Missile::SM::flying::DESTROYED_MINE}
        case DESTROYED_MINE_SIG: {
            static QP::QMTranActTable const tatbl_ = { // transition-action table
                &armed_s,
                {
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            AO_Ship->POST(e, me);
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Missile::SM::exploding} .............................................
QP::QMState const Missile::exploding_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&exploding),
    Q_ACTION_CAST(&exploding_e),
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Missile::SM::exploding}
QP::QState Missile::exploding_e(Missile * const me) {
    me->m_exp_ctr = 0U;
    return QM_ENTRY(&exploding_s);
}
// ${AOs::Missile::SM::exploding}
QP::QState Missile::exploding(Missile * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Missile::SM::exploding::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Missile::SM::exploding::TIME_TICK::[(me->m_x>=GAME_SPEED_X)&&(me->m~}
            if ((me->m_x >= GAME_SPEED_X) && (me->m_exp_ctr < 15U)) {
                ++me->m_exp_ctr;           // advance the explosion counter
                me->m_x -= GAME_SPEED_X;   // move the explosion by one step

                // tell the Tunnel to render the current stage of Explosion
                ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, EXPLOSION_SIG);
                oie->x   = me->m_x + 3U;   // x-pos of explosion
                oie->y   = (int8_t)((int)me->m_y - 4U); // y-pos
                oie->bmp = EXPLOSION0_BMP + (me->m_exp_ctr >> 2);
                AO_Tunnel->POST(oie, me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Missile::SM::exploding::TIME_TICK::[else]}
            else {
                static QP::QMTranActTable const tatbl_ = { // transition-action table
                    &armed_s,
                    {
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

} // namespace GAME
