#pragma once

#include <QtCore/QObject>
#include "interface.h"

class ChatSDKModuleInterface : public PluginInterface
{
public:
    virtual ~ChatSDKModuleInterface() {}
    
    // Client Lifecycle
    Q_INVOKABLE virtual bool initChat(const QString &configJson) = 0;
    Q_INVOKABLE virtual bool startChat() = 0;
    Q_INVOKABLE virtual bool stopChat() = 0;
    Q_INVOKABLE virtual bool destroyChat() = 0;
    Q_INVOKABLE virtual bool setEventCallback() = 0;
    
    // Client Info
    Q_INVOKABLE virtual bool getId() = 0;
    Q_INVOKABLE virtual bool getDefaultInboxId() = 0;
    
    // Conversation Operations
    Q_INVOKABLE virtual bool listConversations() = 0;
    Q_INVOKABLE virtual bool getConversation(const QString &convoId) = 0;
    Q_INVOKABLE virtual bool newPrivateConversation(const QString &introBundleJson, const QString &contentHex) = 0;
    Q_INVOKABLE virtual bool sendMessage(const QString &convoId, const QString &contentHex) = 0;
    
    // Identity Operations
    Q_INVOKABLE virtual bool getIdentity() = 0;
    Q_INVOKABLE virtual bool createIntroBundle() = 0;

signals:
    void eventResponse(const QString& eventName, const QVariantList& data);
};

#define ChatSDKModuleInterface_iid "org.logos.ChatSDKModuleInterface"
Q_DECLARE_INTERFACE(ChatSDKModuleInterface, ChatSDKModuleInterface_iid)
