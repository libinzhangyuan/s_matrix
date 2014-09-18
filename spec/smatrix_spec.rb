require 'spec_helper'

describe SMatrix do
  describe 'test' do
    it '' do
      expect(SMatrix.test).to eq(3)
    end
  end

  describe 'get i set i' do
    it '' do
      p SMatrix.class
      p SMatrix.methods
      a = SMatrix.new
      expect(a.i).to eq(0)
      a.i = 3
      expect(a.i).to eq(3)
    end
  end
end
