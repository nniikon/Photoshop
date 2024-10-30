#ifndef PLUGIN_EXAMPLE_HPP
#define PLUGIN_EXAMPLE_HPP

extern "C" {

/* plugins must provide these two functions */
bool   loadPlugin();
void unloadPlugin();

}

#endif // PLUGIN_EXAMPLE_HPP
