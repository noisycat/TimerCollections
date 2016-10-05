#include <vector>
#include <map>
#include <string>
#include <iostream>

using std::vector;
using std::map;
using std::pair;
using std::string;
using std::unique_ptr;
using std::ostream;

#include <chrono>
using std::chrono::steady_clock;
using std::chrono::duration;

class defaultStopWatch {
	protected:
		bool hasStarted = false;
		bool hasStopped = false;

	public:
		virtual inline bool Started() const{ return hasStarted; };
		virtual inline bool Stopped() const{ return hasStopped; };
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual double Elapsed() = 0;
};

class defaultTimer : public defaultStopWatch {
	private:
		std::chrono::time_point<std::chrono::steady_clock> t1, t0;

	public:
		virtual void Start(){
			hasStarted = true;
			t0 = steady_clock::now();
		};
		virtual void Stop(){
			t1 = steady_clock::now();
			hasStopped = true;
		};
		virtual double Elapsed(){
			if ( !Started() ) return std::chrono::duration<double, std::milli>(0).count()/1000;
			if ( !Stopped() ) this->Stop();
			return std::chrono::duration<double, std::milli>(t1-t0).count()/1000;
		};
};

template<typename T = defaultTimer> class TimerCollections {
	private:
		T* LastTimer;
		map< string, vector< pair< T, string > > > data;

	public:
		TimerCollections() : LastTimer(NULL), data() {};

		~TimerCollections() {};

		T* Add(string key, string notes = "") { 
			pair<T,string> entry(T(), notes);
			auto iter = data.find(key);
			if ( iter == data.end() )
			{
				data[key]=vector< pair<T,string> >(1,std::move(entry));
			} else 
			{
				data[key].push_back(entry);
			}
			LastTimer = &data[key].back().first;
			return LastTimer;
		};

		T* Activate(const string& key, const string notes = "")
		{
			auto iter = data.find(key);
			if( iter != data.end() ) {
				LastTimer = &data[key].back().first;
			}
		}

		T* Start() {
			LastTimer->Start();
			return LastTimer;
		};

		T* Stop() {
			LastTimer->Stop();
			return LastTimer;
		};

		T* Start(const string& key, string notes="") {
			this->Activate(key, notes);
			return this->Start();
		};

		T* Stop(const string& key, string notes="") {
			this->Activate(key, notes);
			return this->Stop();
		};

		friend ostream& operator<<(ostream& os, TimerCollections<T>& TC){
			for(auto key_iter = TC.data.begin(); key_iter != TC.data.end(); ++key_iter)
			{
				os << key_iter->first << "\n";
				for(auto field_iter = key_iter->second.begin(); field_iter != key_iter->second.end(); ++field_iter)
				{
					os << " " << field_iter->first.Elapsed() << "," << field_iter->second << "\n";
				}
			}
			return os;
		};
};
