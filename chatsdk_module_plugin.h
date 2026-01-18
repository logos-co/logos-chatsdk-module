#pragma once

#include <QtCore/QObject>
#include "chatsdk_module_interface.h"
#include "logos_api.h"
#include "logos_api_client.h"
#include "libchat.h"

class ChatSDKModulePlugin : public QObject, public ChatSDKModuleInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ChatSDKModuleInterface_iid FILE "metadata.json")
    Q_INTERFACES(ChatSDKModuleInterface PluginInterface)

public:
    ChatSDKModulePlugin();
    ~ChatSDKModulePlugin();

    // Client Lifecycle
    Q_INVOKABLE bool initChat(const QString &configJson) override;
    Q_INVOKABLE bool startChat() override;
    Q_INVOKABLE bool stopChat() override;
    Q_INVOKABLE bool destroyChat() override;
    Q_INVOKABLE bool setEventCallback() override;
    
    // Client Info
    Q_INVOKABLE bool getId() override;
    Q_INVOKABLE bool getDefaultInboxId() override;
    
    // Conversation Operations
    Q_INVOKABLE bool listConversations() override;
    Q_INVOKABLE bool getConversation(const QString &convoId) override;
    Q_INVOKABLE bool newPrivateConversation(const QString &introBundleJson, const QString &contentHex) override;
    Q_INVOKABLE bool sendMessage(const QString &convoId, const QString &contentHex) override;
    
    // Identity Operations
    Q_INVOKABLE bool getIdentity() override;
    Q_INVOKABLE bool createIntroBundle() override;
    
    QString name() const override { return "chatsdk_module"; }
    QString version() const override { return "1.0.0"; }

    // LogosAPI initialization
    Q_INVOKABLE void initLogos(LogosAPI* logosAPIInstance);

signals:
    void eventResponse(const QString& eventName, const QVariantList& data);

private:
    void* chatCtx;
    
    // Static callback functions for chat SDK
    static void init_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void start_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void stop_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void destroy_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void event_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void get_id_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void get_default_inbox_id_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void list_conversations_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void get_conversation_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void new_private_conversation_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void send_message_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void get_identity_callback(int callerRet, const char* msg, size_t len, void* userData);
    static void create_intro_bundle_callback(int callerRet, const char* msg, size_t len, void* userData);
};
