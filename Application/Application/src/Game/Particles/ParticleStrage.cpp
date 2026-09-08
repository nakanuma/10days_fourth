#include "ParticleStrage.h"

//c++
#include <cassert>

ParticleStrage* ParticleStrage::GetInstance() {
	static ParticleStrage particleEffectManager;
	return &particleEffectManager;
}

void ParticleStrage::Regist(const std::string& _name, Particle* particle) {
	//名前が重複している場合はエラー
	assert(effects_.count(_name) == 0 && "cannot use this name");

	// 引数の名前をキーにしてマップへ格納
	effects_[_name] = particle;
}
