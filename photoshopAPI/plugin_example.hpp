#ifndef PLUGIN_EXAMPLE_HPP
#define PLUGIN_EXAMPLE_HPP

extern "C"
{
/* plugins must provide these two functions */
bool   onLoadPlugin();
void onUnloadPlugin();

}

#endif // PLUGIN_EXAMPLE_HPP
