#pragma once
//顶点缓冲区,存储所有顶点数据
class VertexBuffer
{
private:
	//内部渲染器id
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	//锁定和解锁资源？
	void Bind() const;
	void Unbind() const;
};