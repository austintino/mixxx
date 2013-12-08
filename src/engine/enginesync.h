/***************************************************************************
                          enginesync.h  -  master sync control for
                          maintaining beatmatching amongst n decks
                             -------------------
    begin                : Mon Mar 12 2012
    copyright            : (C) 2012 by Owen Williams
    email                : owilliams@mixxx.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef ENGINESYNC_H
#define ENGINESYNC_H

#include "engine/enginecontrol.h"

class EngineChannel;
class ControlObject;
class ControlPushButton;
class ControlPotmeter;
class RateControl;

enum SYNC_MODE {
    SYNC_NONE = 0,
    SYNC_FOLLOWER = 1,
    SYNC_MASTER = 2
};

class EngineSync : public EngineControl {
    Q_OBJECT

  public:
    explicit EngineSync(ConfigObject<ConfigValue>* pConfig);
    virtual ~EngineSync();

    void addChannel(EngineChannel* pChannel);
    void addDeck(RateControl* pRate);
    EngineChannel* getMaster() const;
    void process(int bufferSize);
    RateControl* getRateControlForGroup(const QString& group);
    const QString getSyncSource() const { return m_sSyncSource; }
    // Used by RateControl to tell the master sync it wants to be enabled.
    void setChannelSyncMode(RateControl* pRateControl, int state);
    // Similar, but will accept master or follower mode.
    void setChannelSyncMode(RateControl* pRateControl);
    void setChannelRateSlider(RateControl* pRateControl, double new_bpm);
    void setDeckPlaying(RateControl* pRateControl, bool state);

  private slots:
    void slotMasterBpmChanged(double);
    void slotSyncRateSliderChanged(double);
    void slotSourceRateEngineChanged(double);
    void slotSourceBpmChanged(double);
    void slotSourceBeatDistanceChanged(double);
    void slotSampleRateChanged(double);
    void slotInternalMasterChanged(double);

  private:
    int playingSyncDeckCount();
    void setMaster(const QString& group);
    bool setChannelMaster(RateControl* pRateControl);
    void setInternalMaster();
    void chooseNewMaster(const QString& dontpick);
    void disableCurrentMaster();
    void updateSamplesPerBeat();
    void setPseudoPosition(double percent);
    void initializeInternalBeatDistance();
    void initializeInternalBeatDistance(RateControl* pRateControl);
    double getInternalBeatDistance() const;

    ConfigObject<ConfigValue>* m_pConfig;

    RateControl* m_pChannelMaster;

    ControlObject* m_pMasterBpm;
    ControlObject* m_pMasterBeatDistance;
    ControlObject* m_pSampleRate;
    ControlPushButton* m_pSyncInternalEnabled;
    ControlPotmeter* m_pInternalRateSlider;

    QList<RateControl*> m_ratecontrols;
    QString m_sSyncSource;
    bool m_bExplicitMasterSelected;
    double m_dSamplesPerBeat;

    // Used for maintaining internal master sync.
    double m_dPseudoBufferPos;
};

#endif
