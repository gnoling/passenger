/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#ifndef _PASSENGER_RESOURCE_LOCATOR_H_
#define _PASSENGER_RESOURCE_LOCATOR_H_

#include <string>
#include <IniFile.h>
#include <Exceptions.h>
#include <Utils.h>

namespace Passenger {

using namespace boost;


/**
 * Locates various Phusion Passenger resources on the filesystem.
 */
class ResourceLocator {
private:
	string binDir;
	string agentsDir;
	string helperScriptsDir;
	string resourcesDir;
	string docDir;
	string rubyLibDir;
	string compilableSourceDir;
	string headerDir;
	string apache2Module;
	
	string getOption(const string &file, const IniFileSectionPtr &section, const string &key) const {
		if (section->hasKey(key)) {
			return section->get(key);
		} else {
			throw RuntimeException("Option '" + key + "' missing in file " + file);
		}
	}
	
public:
	ResourceLocator(const string &rootOrFile) {
		if (getFileType(rootOrFile) == FT_REGULAR) {
			string file = rootOrFile;
			IniFileSectionPtr options = IniFile(file).section("locations");
			binDir              = getOption(file, options, "bin");
			agentsDir           = getOption(file, options, "agents");
			helperScriptsDir    = getOption(file, options, "helper_scripts");
			resourcesDir        = getOption(file, options, "resources");
			docDir              = getOption(file, options, "doc");
			rubyLibDir          = getOption(file, options, "rubylib");
			compilableSourceDir = getOption(file, options, "compilable_source");
			headerDir           = getOption(file, options, "headers");
			apache2Module       = getOption(file, options, "apache2_module");
		} else {
			string root = rootOrFile;
			bool nativelyPackaged = !fileExists(root + "/Rakefile") ||
				!fileExists(root + "/DEVELOPERS.TXT");
			
			if (nativelyPackaged) {
				binDir              = "/usr/bin";
				agentsDir           = "/usr/lib/phusion-passenger/agents";
				helperScriptsDir    = "/usr/share/phusion-passenger/helper-scripts";
				resourcesDir        = "/usr/share/phusion-passenger";
				docDir              = "/usr/share/doc/phusion-passenger";
				rubyLibDir          = "";
				compilableSourceDir = "/usr/share/phusion-passenger/compilable-source";
				headerDir           = "/usr/include/phusion-passenger";
				apache2Module       = "/usr/lib/apache2/modules/mod_passenger.so";
			} else {
				binDir              = root + "/bin";
				agentsDir           = root + "/agents";
				helperScriptsDir    = root + "/helper-scripts";
				resourcesDir        = root + "/resources";
				docDir              = root + "/doc";
				rubyLibDir          = root + "/lib";
				compilableSourceDir = root;
				headerDir           = root + "/ext";
				apache2Module       = root + "/ext/apache2/mod_passenger.so";
			}
		}
	}
	
	string getAgentsDir() const {
		return agentsDir;
	}
	
	string getHelperScriptsDir() const {
		return helperScriptsDir;
	}
	
	string getSpawnServerFilename() const {
		return getHelperScriptsDir() + "/passenger-spawn-server";
	}
	
	string getResourcesDir() const {
		return resourcesDir;
	}
	
	string getDocDir() const {
		return docDir;
	}
	
	// Can be empty.
	string getRubyLibDir() const {
		return rubyLibDir;
	}
	
	// Directory must contain native_support and Nginx module.
	string getCompilableSourceDir() const {
		return compilableSourceDir;
	}
	
	// Directory must contain ext/boost, ext/oxt and ext/common headers.
	string getHeaderDir() const {
		return headerDir;
	}
	
	string getApache2ModuleFilename() const {
		return apache2Module;
	}
};


}

#endif /* _PASSENGER_RESOURCE_LOCATOR_H_ */
