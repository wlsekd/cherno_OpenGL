#pragma once
//索引缓冲区，当前绘制需要的顶点数据索引
class IndexBuffer
{
private:
	//内部渲染器id
	unsigned int m_RendererID;
	//记录索引数量
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	//锁定和解锁资源？
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
};