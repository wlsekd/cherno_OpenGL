#pragma once
//���㻺����,�洢���ж�������
class VertexBuffer
{
private:
	//�ڲ���Ⱦ��id
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	//�����ͽ�����Դ��
	void Bind() const;
	void Unbind() const;
};