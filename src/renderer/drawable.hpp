
#pragma once

class drawable {
public:
    virtual ~drawable() = default;
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
};