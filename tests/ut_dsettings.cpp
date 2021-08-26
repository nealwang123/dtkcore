/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     Wang Fei <wangfeia@uniontech.com>
 *
 * Maintainer: Wang Fei <wangfeia@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include "settings/dsettings.h"
#include "settings/dsettingsoption.h"
#include "settings/dsettingsgroup.h"

DCORE_USE_NAMESPACE


class ut_DSettings : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DSettings *settings;
    QString jsonContent;
};

void ut_DSettings::SetUp()
{
    settings = new DSettings;
    QFile file("/tmp/test.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    jsonContent = " { \"groups\": [{ "
                  " \"key\": \"base\", "
                  " \"name\": \"Basic settings\", "
                  " \"groups\": [{ "
                  " \"key\": \"open_action\", "
                  " \"name\": \"Open Action\", "
                  " \"options\": [{ "
                  " \"key\": \"alway_open_on_new\", "
                  " \"type\": \"checkbox\", "
                  " \"text\": \"Always Open On New Windows\", "
                  " \"default\": true "
                  " }]  "
                  " }] }]}";
    out << jsonContent;
    file.close();
}

void ut_DSettings::TearDown()
{
    if (settings) {
        delete settings;
        settings = nullptr;
    }
    QFile file("/tmp/test.json");
    if (file.exists())
        file.remove();
}

TEST_F(ut_DSettings, testDSettingFromJson)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJson(jsonContent.toLatin1());
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    auto keys = scopeSettings->keys();
    ASSERT_TRUE(!keys.isEmpty());
}

TEST_F(ut_DSettings, testDSettingFromJsonFile)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList keys = scopeSettings->keys();
    ASSERT_TRUE(!keys.isEmpty());
}

TEST_F(ut_DSettings, testDSettingMeta)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QJsonObject jsonObject = scopeSettings->meta();
    ASSERT_TRUE(!jsonObject.isEmpty());
}

TEST_F(ut_DSettings, testDSettingKeys)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList keys = scopeSettings->keys();
    ASSERT_TRUE(!keys.isEmpty());
}

TEST_F(ut_DSettings, testDSettingOptions)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QList<QPointer<DSettingsOption>> options = scopeSettings->options();
    ASSERT_TRUE(!options.isEmpty());
}

TEST_F(ut_DSettings, testDSettingOption)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList keys = scopeSettings->keys();
    QPointer<DSettingsOption> option = scopeSettings->option(keys[0]);
    QString optionKey = option->key();
    ASSERT_TRUE(!optionKey.isEmpty());
}

TEST_F(ut_DSettings, testDSettingValue)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList keys = scopeSettings->keys();
    QVariant value = scopeSettings->value(keys[0]);
    ASSERT_TRUE(value.toBool());
}

TEST_F(ut_DSettings, testDSettingGroupKeys)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList groupKeys = scopeSettings->groupKeys();
    ASSERT_TRUE(!groupKeys.isEmpty());
}

TEST_F(ut_DSettings, testDSettingGroups)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QList<QPointer<DSettingsGroup>> groups = scopeSettings->groups();
    ASSERT_TRUE(!groups.isEmpty());
}

TEST_F(ut_DSettings, testDSettingGroup)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList keys = scopeSettings->keys();
    QPointer<DSettingsGroup> group = scopeSettings->group("base.open_action");
    QString optionKey = group->key();
    ASSERT_TRUE(!optionKey.isEmpty());
}

TEST_F(ut_DSettings, testDSettingGetOption)
{
    QPointer<DSettings> tmpSetting = DSettings::fromJsonFile("/tmp/test.json");
    QScopedPointer<DSettings> scopeSettings(tmpSetting.data());
    QStringList keys = scopeSettings->keys();
    QVariant option = scopeSettings->getOption(keys[0]);
    ASSERT_TRUE(option.toBool());
}