#pragma once

namespace Scheduler
{
    class PluginContext;

    class Plugin
    {
    public:
        virtual ~Plugin(){}

        virtual size_t getApiVersion() = 0;

        virtual void install(PluginContext* context) = 0;
        virtual void uninstall(PluginContext* context) = 0;
    };
}