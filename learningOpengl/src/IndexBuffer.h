#pragma once
//��������������ǰ������Ҫ�Ķ�����������
class IndexBuffer
{
private:
	//�ڲ���Ⱦ��id
	unsigned int m_RendererID;
	//��¼��������
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	//�����ͽ�����Դ��
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
};