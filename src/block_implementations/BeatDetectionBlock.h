#ifndef BEATDETECTIONBLOCK_H
#define BEATDETECTIONBLOCK_H

#include "block_data/OneOutputBlock.h"
#include "AudioInputAnalyzer.h"
#include "utils.h"

#include <QPointer>


class BeatDetectionBlock : public OneOutputBlock {

    Q_OBJECT

    Q_PROPERTY(AudioInputAnalyzer* analyzer READ getAnalyzer NOTIFY inputChanged)
    Q_PROPERTY(double bpm READ getBpm NOTIFY bpmChanged)
    Q_PROPERTY(bool bpmIsValid READ getBpmIsValid NOTIFY bpmChanged)
    Q_PROPERTY(int minBpm READ getMinBpm WRITE setMinBpm NOTIFY minBpmChanged)
    Q_PROPERTY(double factor READ getFactor WRITE setFactor NOTIFY factorChanged)

public:

    static BlockInfo info() {
        static BlockInfo info;
        info.nameInUi = "Tempo Detection";
        info.typeName = "Beat Detection";
        info.category << "Sound2Light";
        info.dependencies = {BlockDependency::AudioInput};
        info.helpText = "Tries to detect the tempo of the music in BPM.\n"
                        "The input device can be chosen above.";
        info.qmlFile = "qrc:/qml/Blocks/BeatDetectionBlock.qml";
        info.complete<BeatDetectionBlock>();
        return info;
    }

    explicit BeatDetectionBlock(MainController* m_controller, QString m_uid);
    ~BeatDetectionBlock();

    virtual QJsonObject getState() const override;

    virtual void setState(const QJsonObject& state) override;

signals:
    void inputChanged();
    void bpmChanged();
    void minBpmChanged();
    void factorChanged();

public slots:
    virtual BlockInfo getBlockInfo() const override { return info(); }

    void updateOutput();

    void onBpmChanged();

    QString getInputName() const;
    void setInputByName(QString name);

    AudioInputAnalyzer* getAnalyzer() const { return m_analyzer; }

    double getBpm() const { return m_analyzer ? m_analyzer->getBpm(m_minBpm) : 0; }
    bool getBpmIsValid() const { return m_analyzer ? m_analyzer->getBpmIsValid() : false; }

    int getMinBpm() const { return m_minBpm; }
    void setMinBpm(int value) { m_minBpm = value; emit minBpmChanged(); }

    double getFactor() const { return m_factor; }
    void setFactor(double value);

protected:
    QPointer<AudioInputAnalyzer> m_analyzer;
    int m_minBpm;
    double m_factor;

};

#endif // BEATDETECTIONBLOCK_H
