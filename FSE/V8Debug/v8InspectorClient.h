#ifndef V8INSPECTORCLIENTIMPL_H
#define V8INSPECTORCLIENTIMPL_H

#include <iostream>
#include <v8.h>
#include <v8-inspector.h>
#include <libplatform/libplatform.h>
#include "v8InspectorChannel.h"


class V8InspectorClientImpl final : public v8_inspector::V8InspectorClient {
public:
	V8InspectorClientImpl(v8::Platform* platform, const v8::Local<v8::Context>& context,
		const std::function<void(std::string)>& onResponse,
		const std::function<void(void)>& onWaitFrontendMessageOnPause);

	void dispatchProtocolMessage(const v8_inspector::StringView& message_view) const;
	void runMessageLoopOnPause(int contextGroupId) override;
	void quitMessageLoopOnPause() override;
	void schedulePauseOnNextStatement(const v8_inspector::StringView& reason) const;
	bool isWaitFrontendMessageMessageOnPause() const;
	void waitFrontendMessageOnPause();
	void quit();
private:
	v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

	static const int kContextGroupId = 1;
	v8::Platform* platform_;
	std::unique_ptr<v8_inspector::V8Inspector> inspector_;
	std::unique_ptr<v8_inspector::V8InspectorSession> session_;
	std::unique_ptr<V8InspectorChannelImp> channel_;
	v8::Isolate* isolate_;
	v8::Handle<v8::Context> context_;
	std::function<void(void)> onWaitFrontendMessageOnPause_;
	bool terminated_ = false;
	bool run_nested_loop_ = false;
};

#endif // V8INSPECTORCLIENTIMPL_H
