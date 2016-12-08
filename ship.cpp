//****************************************************************************
// Model: game.qm
// File:  ./ship.cpp
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
//${.::ship.cpp} .............................................................
#include "qpcpp.h"
#include "bsp.h"
#include "game.h"

//Q_DEFINE_THIS_FILE

#define SHIP_WIDTH  5U
#define SHIP_HEIGHT 3U

// encapsulated delcaration of the Ship active object ------------------------
namespace GAME {

//${AOs::Ship} ...............................................................
class Ship : public QP::QMActive {
private:
    uint8_t m_x;
    uint16_t m_y;
    uint8_t m_exp_ctr;
    uint16_t m_score;

public:
    Ship();

protected:
    static QP::QState initial(Ship * const me, QP::QEvt const * const e);
    static QP::QState active  (Ship * const me, QP::QEvt const * const e);
    static QP::QState active_i(Ship * const me);
    static QP::QMState const active_s;
    static QP::QState parked  (Ship * const me, QP::QEvt const * const e);
    static QP::QMState const parked_s;
    static QP::QState flying  (Ship * const me, QP::QEvt const * const e);
    static QP::QState flying_e(Ship * const me);
    static QP::QMState const flying_s;
    static QP::QState exploding  (Ship * const me, QP::QEvt const * const e);
    static QP::QState exploding_e(Ship * const me);
    static QP::QMState const exploding_s;
};

} // namespace GAME

namespace GAME {

// local objects -------------------------------------------------------------
static Ship l_ship; // the sole instance of the Ship active object

// Public-scope objects ------------------------------------------------------
QP::QMActive * const AO_Ship = &l_ship; // opaque pointer

} // namespace GAME

// Active object definition --------------------------------------------------
namespace GAME {

//${AOs::Ship} ...............................................................
//${AOs::Ship::Ship} .........................................................
Ship::Ship()
  : QMActive(Q_STATE_CAST(&Ship::initial)),
    m_x(GAME_SHIP_X),
    m_y(GAME_SHIP_Y << 2)
{}

//${AOs::Ship::SM} ...........................................................
QP::QState Ship::initial(Ship * const me, QP::QEvt const * const e) {
    static struct {
        QP::QMState const *target;
        QP::QActionHandler act[2];
    } const tatbl_ = { // transition-action table
        &active_s,
        {
            Q_ACTION_CAST(&active_i), // initial tran.
            Q_ACTION_CAST(0)  // zero terminator
        }
    };
    // ${AOs::Ship::SM::initial}
    me->subscribe(TIME_TICK_SIG);
    me->subscribe(PLAYER_TRIGGER_SIG);
    // object dictionaries...
    QS_OBJ_DICTIONARY(&l_ship);
    // function dictionaries for Ship HSM...
    QS_FUN_DICTIONARY(&Ship::initial);
    QS_FUN_DICTIONARY(&Ship::active);
    QS_FUN_DICTIONARY(&Ship::parked);
    QS_FUN_DICTIONARY(&Ship::flying);
    QS_FUN_DICTIONARY(&Ship::exploding);
    // local signals...
    QS_SIG_DICTIONARY(PLAYER_SHIP_MOVE_SIG, &l_ship);
    QS_SIG_DICTIONARY(TAKE_OFF_SIG,         &l_ship);
    QS_SIG_DICTIONARY(HIT_WALL_SIG,         &l_ship);
    QS_SIG_DICTIONARY(HIT_MINE_SIG,         &l_ship);
    QS_SIG_DICTIONARY(DESTROYED_MINE_SIG,   &l_ship);

    (void)e; // unused parameter
    return QM_TRAN_INIT(&tatbl_);
}
//${AOs::Ship::SM::active} ...................................................
QP::QMState const Ship::active_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&active),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(&active_i)
};
// ${AOs::Ship::SM::active::initial}
QP::QState Ship::active_i(Ship * const me) {
    static QP::QMTranActTable const tatbl_ = { // transition-action table
        &parked_s,
        {
            Q_ACTION_CAST(0)  // zero terminator
        }
    };
    // ${AOs::Ship::SM::active::initial}
    return QM_TRAN_INIT(&tatbl_);
}
// ${AOs::Ship::SM::active}
QP::QState Ship::active(Ship * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    (void)me; // avoid compiler warning in case 'me' is not used
    return status_;
}
//${AOs::Ship::SM::active::parked} ...........................................
QP::QMState const Ship::parked_s = {
    &Ship::active_s, // superstate
    Q_STATE_CAST(&parked),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Ship::SM::active::parked}
QP::QState Ship::parked(Ship * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Ship::SM::active::parked::TAKE_OFF}
        case TAKE_OFF_SIG: {
            static struct {
                QP::QMState const *target;
                QP::QActionHandler act[2];
            } const tatbl_ = { // transition-action table
                &flying_s,
                {
                    Q_ACTION_CAST(&flying_e), // entry
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
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
//${AOs::Ship::SM::active::flying} ...........................................
QP::QMState const Ship::flying_s = {
    &Ship::active_s, // superstate
    Q_STATE_CAST(&flying),
    Q_ACTION_CAST(&flying_e),
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Ship::SM::active::flying}
QP::QState Ship::flying_e(Ship * const me) {
    me->m_score = 0U; /* reset the score */
    ScoreEvt *sev = Q_NEW(ScoreEvt, SCORE_SIG);
    sev->score = me->m_score;
    AO_Tunnel->POST(sev, me);
    return QM_ENTRY(&flying_s);
}
// ${AOs::Ship::SM::active::flying}
QP::QState Ship::flying(Ship * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Ship::SM::active::flying::TIME_TICK}
        case TIME_TICK_SIG: {
            if (BSP_isThrottle()) {
                if (me->m_y > 0U) {
                    me->m_y -= 1U;
                }
            }
            else {
                if (me->m_y < (GAME_TUNNEL_HEIGHT << 2)) {
                    me->m_y += 1U;
                }
            }

            // tell the Tunnel to draw the Ship and test for hits
            ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, SHIP_IMG_SIG);
            oie->x   = me->m_x;
            oie->y   = me->m_y >> 2;
            oie->bmp = SHIP_BMP;
            AO_Tunnel->POST(oie, me);

            ++me->m_score; // increment the score for surviving another tick

            if ((me->m_score % 10U) == 0U) { // is the score "round"?
                ScoreEvt *sev = Q_NEW(ScoreEvt, SCORE_SIG);
                sev->score = me->m_score;
                AO_Tunnel->POST(sev, me);
            }
            status_ = QM_HANDLED();
            break;
        }
        // ${AOs::Ship::SM::active::flying::PLAYER_TRIGGER}
        case PLAYER_TRIGGER_SIG: {
            ObjectPosEvt *ope = Q_NEW(ObjectPosEvt, MISSILE_FIRE_SIG);
            ope->x = me->m_x;
            ope->y = (me->m_y >> 2) + SHIP_HEIGHT - 1U;
            AO_Missile->POST(ope, me);
            status_ = QM_HANDLED();
            break;
        }
        // ${AOs::Ship::SM::active::flying::DESTROYED_MINE}
        case DESTROYED_MINE_SIG: {
            me->m_score += Q_EVT_CAST(ScoreEvt)->score;
            // the score will be sent to the Tunnel by the next TIME_TICK
            status_ = QM_HANDLED();
            break;
        }
        // ${AOs::Ship::SM::active::flying::HIT_WALL}
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
        // ${AOs::Ship::SM::active::flying::HIT_MINE}
        case HIT_MINE_SIG: {
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
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Ship::SM::active::exploding} ........................................
QP::QMState const Ship::exploding_s = {
    &Ship::active_s, // superstate
    Q_STATE_CAST(&exploding),
    Q_ACTION_CAST(&exploding_e),
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Ship::SM::active::exploding}
QP::QState Ship::exploding_e(Ship * const me) {
    me->m_exp_ctr = 0U;
    return QM_ENTRY(&exploding_s);
}
// ${AOs::Ship::SM::active::exploding}
QP::QState Ship::exploding(Ship * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Ship::SM::active::exploding::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Ship::SM::active::exploding::TIME_TICK::[me->m_exp_ctr<15U]}
            if (me->m_exp_ctr < 15U) {
                ++me->m_exp_ctr;
                // tell the Tunnel to draw the current stage of Explosion
                ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, EXPLOSION_SIG);
                oie->bmp = EXPLOSION0_BMP + (me->m_exp_ctr >> 2);
                oie->x   = me->m_x; // x of explosion
                oie->y   = (int8_t)((int)(me->m_y >> 2) - 4U + SHIP_HEIGHT);
                AO_Tunnel->POST(oie, me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Ship::SM::active::exploding::TIME_TICK::[else]}
            else {
                static QP::QMTranActTable const tatbl_ = { // transition-action table
                    &parked_s,
                    {
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                ScoreEvt *gameOver = Q_NEW(ScoreEvt, GAME_OVER_SIG);
                gameOver->score = me->m_score;
                AO_Tunnel->POST(gameOver, me);
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
