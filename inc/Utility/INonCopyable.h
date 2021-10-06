#pragma once

//コピー禁止用クラス private継承するとクラスのコピーを禁止できます
class INonCopyable
{
protected:
	INonCopyable() {};
	virtual ~INonCopyable() {};

private:
	//コピー禁止
	void operator =(const INonCopyable& src) = delete;
	INonCopyable(const INonCopyable& src) = delete;
};