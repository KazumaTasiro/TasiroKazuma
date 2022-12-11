#include"Sprite.h"

void Sprite::Initialize(SpriteCommon* spritecommon_)
{
	
	spritecomon = spritecommon_;
	
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = spritecomon->GetDxCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	spritecomon->GetDxCommon()->GetDevice()->CreateShaderResourceView(spritecomon->GetTexBuff(), &srvDesc, spritecomon->GetSrvHandle());
}

void Sprite::Draw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->SetPipelineState(spritecomon->GetPipelineState());
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootSignature(spritecomon->GetRootSignature());
	//�v���~�e�B�u�`��̐ݒ�R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//�O�p���X�g
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, spritecomon->GetConstBuffMaterial()->GetGPUVirtualAddress());

	// SRV�q�[�v�̐ݒ�R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->SetDescriptorHeaps(1, &spritecomon->srvHeap);
	// SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spritecomon->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, spritecomon->GetConstBuffTransform()->GetGPUVirtualAddress());

	// �`��R�}���h
	spritecomon->GetDxCommon()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}
