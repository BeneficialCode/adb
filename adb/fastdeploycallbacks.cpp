#define TRACE_TAG ADB

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "file_sync_client.h"
#include "commandline.h"
#include "sysdeps.h"

#include "fastdeploycallbacks.h"

static void appendBuffer(std::vector<char>* buffer, const char* input, int length) {
    if (buffer != NULL) {
        buffer->insert(buffer->end(), input, input + length);
    }
}

class DeployAgentBufferCallback : public StandardStreamsCallbackInterface {
public:
    DeployAgentBufferCallback(std::vector<char>* outBuffer, std::vector<char>* errBuffer);

    virtual bool OnStdout(const char* buffer, size_t length);
    virtual bool OnStderr(const char* buffer, size_t length);
    virtual int Done(int status);

private:
    std::vector<char>* mpOutBuffer;
    std::vector<char>* mpErrBuffer;
};

int capture_shell_command(const char* command, std::vector<char>* outBuffer,
    std::vector<char>* errBuffer) {
    DeployAgentBufferCallback cb(outBuffer, errBuffer);
    return send_shell_command(command, /*disable_shell_protocol=*/false, &cb);
}

DeployAgentBufferCallback::DeployAgentBufferCallback(std::vector<char>* outBuffer,
    std::vector<char>* errBuffer) {
    mpOutBuffer = outBuffer;
    mpErrBuffer = errBuffer;
}

bool DeployAgentBufferCallback::OnStdout(const char* buffer, size_t length) {
    appendBuffer(mpOutBuffer, buffer, length);
    return true;
}

bool DeployAgentBufferCallback::OnStderr(const char* buffer, size_t length) {
    appendBuffer(mpErrBuffer, buffer, length);
    return true;
}

int DeployAgentBufferCallback::Done(int status) {
    return status;
}