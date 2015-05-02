class RemoveChampsLibreIntFromQuestions < ActiveRecord::Migration
  def change
    remove_column :questions, :champs_libre_int, :integer
  end
end
